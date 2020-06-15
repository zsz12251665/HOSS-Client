#include "RemoteAPI.h"

#include "RemoteAPI_ProgressDialog.h"
#include "Settings.h"

#include <QDebug>
#include <QFileDialog>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRandomGenerator>

static QHttpPart makePart(const QString type, const QString disposition, const QString body)
{
	QHttpPart httpPart;
	httpPart.setRawHeader("Content-Type", type.toUtf8());
	httpPart.setRawHeader("Content-Disposition", disposition.toUtf8());
	httpPart.setBody(body.toUtf8());
	return httpPart;
}

const QString RemoteAPI::formBoundary = QString("--%1_Boundary_%1--").arg(
			QRandomGenerator::global()->generate64(), 16, 16, QLatin1Char('0'));

RemoteAPI::RemoteAPI(const QUrl serverURL) :
	serverURL(serverURL.adjusted(QUrl::NormalizePathSegments).adjusted(QUrl::RemoveFilename))
{
	manager = new QNetworkAccessManager;
	QObject::connect(manager, &QNetworkAccessManager::finished,
					 &waitUntilFinished, &QEventLoop::quit);
}

RemoteAPI::~RemoteAPI()
{
	delete manager;
}

QJsonArray RemoteAPI::fetchRemoteToDos()
{
	qDebug() << "RemoteAPI::fetchRemoteToDos() Starts";
	// Send the request
	QNetworkRequest request(QString(serverURL.toEncoded() + "homework.php"));
	QNetworkReply *reply = manager->get(request);
	// Wait for the reply
	waitUntilFinished.exec();
	// Transfer the response to an array
	QJsonArray responseJSON = QJsonDocument::fromJson(reply->readAll()).array();
	// Close the objects & return
	delete reply;
	qDebug() << responseJSON.size();
	for (int i = 0; i < responseJSON.size(); ++i)
		qDebug() << responseJSON.at(i)["title"].toString() << responseJSON.at(i)["deadline"].toString();
	qDebug() << "RemoteAPI::fetchRemoteToDos() Ends" << endl;
	return responseJSON;
}

int RemoteAPI::uploadHomework(const QString name, const QString number,
								  const QString directory, const QString filename)
{
	qDebug() << "RemoteAPI::uploadHomework() Starts";
	// Fulfill the form
	QHttpMultiPart *form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	form->setBoundary(formBoundary.toUtf8());
	// Append the information
	form->append(makePart("text/plain", "form-data; name=\"StuName\"", name));
	form->append(makePart("text/plain", "form-data; name=\"StuNumber\"", number));
	form->append(makePart("text/plain", "form-data; name=\"WorkTitle\"", directory));
	// Append the homework file
	QFile homework(filename);
	homework.open(QFile::ReadOnly);
	QHttpPart filePart = makePart("application/octet-stream",
								  "form-data; name=\"WorkFile\"; filename=\"" +
								  homework.fileName() + "\"", QString());
	filePart.setBodyDevice(&homework);
	form->append(filePart);
	// Send the request
	QNetworkRequest request(QString(serverURL.toEncoded() + "upload.php"));
	request.setRawHeader("Content-Type",
						 ("multipart/form-data;boundary=" + formBoundary).toUtf8());
	QNetworkReply *reply = manager->post(request, form);
	RemoteAPI_ProgressDialog progress;
	QObject::connect(reply, &QNetworkReply::uploadProgress, &progress,
					 &RemoteAPI_ProgressDialog::updateProgress);
	QObject::connect(manager, &QNetworkAccessManager::finished, &progress,
					 &RemoteAPI_ProgressDialog::close);
	// Show the progress dialog & wait for the reply
	progress.exec();
	// Get the required information
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	QByteArray replyMessage = reply->readAll();
	// Close objects & return
	homework.close();
	delete reply;
	qDebug() << statusCode << replyMessage;
	QMessageBox(QMessageBox::Icon::Information, QString("HTTP %1").arg(statusCode),
				replyMessage, QMessageBox::Ok).exec();
	qDebug() << "RemoteAPI::uploadHomework() Ends" << endl;
	return statusCode;
}

int RemoteAPI::verifySubmission(const QString name, const QString number,
								  const QString directory)
{
	qDebug() << "RemoteAPI::verifySubmission() Starts";
	// Fulfill the form
	QHttpMultiPart *form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	form->setBoundary(formBoundary.toUtf8());
	// Append the information
	form->append(makePart("text/plain", "form-data; name=\"StuName\"", name));
	form->append(makePart("text/plain", "form-data; name=\"StuNumber\"", number));
	form->append(makePart("text/plain", "form-data; name=\"WorkTitle\"", directory));
	// Send the request
	QNetworkRequest request(QString(serverURL.toEncoded() + "verify.php"));
	request.setRawHeader("Content-Type",
						 ("multipart/form-data;boundary=" + formBoundary).toUtf8());
	QNetworkReply *reply = manager->post(request, form);
	// Wait for the reply
	waitUntilFinished.exec();
	// Get the required information
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	qDebug() << statusCode << reply->readAll();
	delete reply;
	qDebug() << "RemoteAPI::verifySubmission() Ends" << endl;
	return statusCode;
}

QJsonArray RemoteAPI::fetchRemoteToDos(const Settings &settings)
{
	return RemoteAPI(settings.getServer()).fetchRemoteToDos();
}

int RemoteAPI::uploadHomework(const Settings &settings, const QString directory)
{
	QString filename = QFileDialog::getOpenFileName(nullptr, "Open the file");
	if (filename.isEmpty())
		return -1;
	return RemoteAPI(settings.getServer()).uploadHomework(settings.getName(),
														  settings.getNumber(),
														  directory, filename);
}

int RemoteAPI::verifySubmission(const Settings &settings, const QString directory)
{
	return RemoteAPI(settings.getServer()).verifySubmission(settings.getName(),
															 settings.getNumber(), directory);
}
