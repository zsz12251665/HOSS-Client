#include "remoteAPI.h"

QString RemoteAPI::formBoundary = QString("--%1_Boundary_%1--").arg(QRandomGenerator::global()->generate64(), 16, 16, QLatin1Char('0'));

RemoteAPI::RemoteAPI(const QUrl serverURL):serverURL(serverURL.adjusted(QUrl::NormalizePathSegments).adjusted(QUrl::RemoveFilename))
{
	manager = new QNetworkAccessManager;
	QObject::connect(manager, &QNetworkAccessManager::finished, &waitUntilFinished, &QEventLoop::quit);
}

RemoteAPI::~RemoteAPI()
{
	delete manager;
}

QJsonArray RemoteAPI::fetchRemoteToDos()
{
	QNetworkRequest request(QString(serverURL.toEncoded() + "homework.php"));
	// Send the request
	QNetworkReply *reply = manager->get(request);
	// Wait for the reply
	waitUntilFinished.exec();
	// Transfer the response to an array
	QJsonDocument responseJSON = QJsonDocument::fromJson(reply->readAll());
	return responseJSON.array();
}

QHttpPart RemoteAPI::makePart(const QString type, const QString disposition, const QString body)
{
	QHttpPart httpPart;
	httpPart.setRawHeader("Content-Type", type.toUtf8());
	httpPart.setRawHeader("Content-Disposition", disposition.toUtf8());
	httpPart.setBody(body.toUtf8());
	return httpPart;
}

QString RemoteAPI::uploadHomework(const QString name, const QString number, const QString directory, QFile &homework)
{
	QNetworkRequest request(QString(serverURL.toEncoded() + "upload.php"));
	request.setRawHeader("Content-Type", ("multipart/form-data;boundary=" + formBoundary).toUtf8());
	// Fulfill the form
	QHttpMultiPart *form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	form->setBoundary(formBoundary.toUtf8());
	// Append the information
	form->append(RemoteAPI::makePart("text/plain", "form-data; name=\"StuName\"", name));
	form->append(RemoteAPI::makePart("text/plain", "form-data; name=\"StuNumber\"", number));
	form->append(RemoteAPI::makePart("text/plain", "form-data; name=\"WorkTitle\"", directory));
	// Append the homework file
	homework.open(QFile::ReadOnly);
	QHttpPart filePart = RemoteAPI::makePart("application/octet-stream", "form-data; name=\"WorkFile\"; filename=\"" + homework.fileName() + "\"");
	filePart.setBodyDevice(&homework);
	form->append(filePart);
	// Send the request
	QNetworkReply *reply = manager->post(request, form);
	// Wait for the reply
	waitUntilFinished.exec();
	homework.close();
	return reply->readAll();
}
