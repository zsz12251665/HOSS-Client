#include "RemoteAPI.h"

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
								  const QString directory)
{
	QString fileName = QFileDialog::getOpenFileName(nullptr, "Open the file");
	QFile homework(fileName);
	return fileName.isEmpty() ? -1 : uploadHomework(name, number, directory, homework);
}

int RemoteAPI::uploadHomework(const QString name, const QString number,
								  const QString directory, QFile &homework)
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
	// Wait for the reply
	waitUntilFinished.exec();
	// Get the required information
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	QByteArray replyMessage = reply->readAll();
	// Close the objects & return
	homework.close();
	delete reply;
	qDebug() << statusCode << replyMessage;
	QMessageBox(QMessageBox::Icon::Information, QString("HTTP %1").arg(statusCode),
				replyMessage, QMessageBox::Ok).exec();
	qDebug() << "RemoteAPI::uploadHomework() Ends" << endl;
	return statusCode;
}
