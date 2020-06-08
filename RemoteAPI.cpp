#include "RemoteAPI.h"

#include <QDebug>
#include <QFileDialog>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonDocument>
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
	// Send the request
	QNetworkRequest request(QString(serverURL.toEncoded() + "homework.php"));
	QNetworkReply *reply = manager->get(request);
	// Wait for the reply
	waitUntilFinished.exec();
	// Transfer the response to an array
	QJsonDocument responseJSON = QJsonDocument::fromJson(reply->readAll());
	// Close the objects & return
	delete reply;
	return responseJSON.array();
}

QPair<int, QByteArray> RemoteAPI::uploadHomework(const QString name, const QString number,
								  const QString directory)
{
	QString fileName = QFileDialog::getOpenFileName(nullptr, "Open the file");
	QFile homework(fileName);
	return fileName.isNull() ? QPair<int, QByteArray>(-1, "No file selected") :
		uploadHomework(name, number, directory, homework);
}

QPair<int, QByteArray> RemoteAPI::uploadHomework(const QString name, const QString number,
								  const QString directory, QFile &homework)
{
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
	qDebug() << statusCode << replyMessage;
	// Close the objects & return
	homework.close();
	delete reply;
	return QPair<int, QByteArray>(statusCode, replyMessage);
}
