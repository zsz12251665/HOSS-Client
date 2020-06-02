#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpPart>
#include <QtNetwork/QHttpMultiPart>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QRandomGenerator>

QJsonArray fetchRemoteToDos(QUrl remoteURL)
{
	QNetworkAccessManager *manager = new QNetworkAccessManager;
	QNetworkRequest request = QNetworkRequest(remoteURL);
	// Send the request & wait for the reply
	QEventLoop waitUntilFinished;
	QObject::connect(manager, &QNetworkAccessManager::finished,
					 &waitUntilFinished, &QEventLoop::quit);
	QNetworkReply *reply = manager->get(request);
	waitUntilFinished.exec();
	// Transfer the response to an array
	QJsonDocument responseJSON = QJsonDocument::fromJson(reply->readAll());
	return responseJSON.array();
}

QString uploadHomework(QUrl remoteURL, QString name, QString number,
					   QString directory, QFile *homework)
{
	QString formBoundary = QString("--%1Boundary%1--").arg(QRandomGenerator::global()->generate64(),16,16,QLatin1Char('0'));
	QNetworkAccessManager *manager = new QNetworkAccessManager;
	QNetworkRequest request = QNetworkRequest(remoteURL);
	request.setRawHeader("Content-Type", ("multipart/form-data;boundary=" + formBoundary).toUtf8());
	// Fulfill the form
	QHttpMultiPart *form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	form->setBoundary(formBoundary.toUtf8());
	QHttpPart dataPart;
	// Append the information
	dataPart.setHeader(QNetworkRequest::ContentTypeHeader,
					   QVariant("text/plain"));
	dataPart.setHeader(QNetworkRequest::ContentDispositionHeader,
					   QVariant("form-data; name=\"StuName\""));
	dataPart.setBody(name.toUtf8());
	form->append(dataPart);
	dataPart.setHeader(QNetworkRequest::ContentDispositionHeader,
					   QVariant("form-data; name=\"StuNumber\""));
	dataPart.setBody(number.toUtf8());
	form->append(dataPart);
	dataPart.setHeader(QNetworkRequest::ContentDispositionHeader,
					   QVariant("form-data; name=\"WorkTitle\""));
	dataPart.setBody(directory.toUtf8());
	form->append(dataPart);
	// Append the homework file
	homework->open(QFile::ReadOnly);
	dataPart.setHeader(QNetworkRequest::ContentTypeHeader,
					   QVariant("application/octet-stream"));
	dataPart.setHeader(QNetworkRequest::ContentDispositionHeader,
					   QVariant("form-data; name=\"WorkFile\"; filename=\""
								+ homework->fileName() + "\""));
	dataPart.setBodyDevice(homework);
	form->append(dataPart);
	// Send the request & wait for the reply
	QEventLoop waitUntilFinished;
	QObject::connect(manager, &QNetworkAccessManager::finished,
					 &waitUntilFinished, &QEventLoop::quit);
	QNetworkReply *reply = manager->post(request, form);
	waitUntilFinished.exec();
	return reply->readAll();
}
