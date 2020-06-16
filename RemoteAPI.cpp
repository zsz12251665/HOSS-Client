#include "RemoteAPI.h"

#include "RemoteAPI_ProgressDialog.h"
#include "Settings.h"

#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QFileDialog>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QNetworkConfigurationManager>
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

RemoteAPI::RemoteAPI(const QUrl serverURL) : manager(new QNetworkAccessManager),
	serverURL(serverURL.adjusted(QUrl::NormalizePathSegments).adjusted(QUrl::RemoveFilename))
{
	;
}

RemoteAPI::~RemoteAPI()
{
	delete manager;
}

QPair<int, QJsonArray> RemoteAPI::fetchRemoteToDos(QHttpMultiPart *form)
{
	qDebug() << "RemoteAPI::fetchRemoteToDos() Starts";
	// Send the request
	QNetworkRequest request(QString(serverURL.toEncoded() + "homework.php"));
	QNetworkReply *reply = manager->post(request, form);
	// Set up the event loop & wait for the reply
	QEventLoop waitUntilFinished;
	QObject::connect(manager, &QNetworkAccessManager::finished,
					 &waitUntilFinished, &QEventLoop::quit);
	waitUntilFinished.exec();
	// Collect the required information
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	QJsonArray responseJSON = QJsonDocument::fromJson(reply->readAll()).array();
	qDebug() << statusCode << responseJSON.size();
	for (int i = 0; i < responseJSON.size(); ++i)
		qDebug() << responseJSON.at(i)["title"].toString()
				<< responseJSON.at(i)["deadline"].toString()
				<< responseJSON.at(i)["directory"].toString()
				<< responseJSON.at(i)["checked"].toString();
	delete reply;
	qDebug() << "RemoteAPI::fetchRemoteToDos() Ends" << endl;
	return qMakePair(statusCode, responseJSON);
}

int RemoteAPI::uploadHomework(QHttpMultiPart *form)
{
	qDebug() << "RemoteAPI::uploadHomework() Starts";
	// Send the request
	QNetworkRequest request(QString(serverURL.toEncoded() + "upload.php"));
	request.setRawHeader("Content-Type",
						 ("multipart/form-data;boundary=" + formBoundary).toUtf8());
	QNetworkReply *reply = manager->post(request, form);
	// Set up the progress dialog & wait for the reply
	RemoteAPI_ProgressDialog progress;
	QObject::connect(reply, &QNetworkReply::uploadProgress, &progress,
					 &RemoteAPI_ProgressDialog::updateProgress);
	QObject::connect(manager, &QNetworkAccessManager::finished, &progress,
					 &RemoteAPI_ProgressDialog::close);
	progress.exec();
	// Collect the required information
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	QString replyMessage = reply->readAll();
	QMessageBox(QMessageBox::Icon::Information, QString("HTTP %1").arg(statusCode),
				replyMessage, QMessageBox::Ok).exec();
	qDebug() << statusCode << replyMessage;
	delete reply;
	qDebug() << "RemoteAPI::uploadHomework() Ends" << endl;
	return statusCode;
}

QPair<int, QJsonArray> RemoteAPI::fetchRemoteToDos(const Settings &settings)
{
	// Check if offline
	if (!isOnline())
		return qMakePair(0, QJsonArray());
	// Fulfill the form
	QHttpMultiPart *form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	form->setBoundary(formBoundary.toUtf8());
	// Append the information
	form->append(makePart("text/plain", "form-data; name=\"StuName\"", settings.getName()));
	form->append(makePart("text/plain", "form-data; name=\"StuNumber\"",
						  settings.getNumber()));
	// Send the request & return
	return RemoteAPI(settings.getServer()).fetchRemoteToDos(form);
}

int RemoteAPI::uploadHomework(const Settings &settings, const QString directory)
{
	// Check if offline
	if (!isOnline())
		return 0;
	// Get the file
	QString filename = QFileDialog::getOpenFileName(nullptr, "Open the file");
	if (filename.isEmpty())
		return -1;
	// Fulfill the form
	QHttpMultiPart *form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	form->setBoundary(formBoundary.toUtf8());
	// Append the information
	form->append(makePart("text/plain", "form-data; name=\"StuName\"", settings.getName()));
	form->append(makePart("text/plain", "form-data; name=\"StuNumber\"",
						  settings.getNumber()));
	form->append(makePart("text/plain", "form-data; name=\"WorkTitle\"", directory));
	// Append the homework file
	QFile homework(filename);
	homework.open(QFile::ReadOnly);
	QHttpPart filePart = makePart("application/octet-stream",
								  "form-data; name=\"WorkFile\"; filename=\"" +
								  homework.fileName() + "\"", QString());
	filePart.setBodyDevice(&homework);
	form->append(filePart);
	homework.close();
	// Send the request & return
	return RemoteAPI(settings.getServer()).uploadHomework(form);
}

bool RemoteAPI::isOnline()
{
	return QNetworkConfigurationManager().isOnline();
}
