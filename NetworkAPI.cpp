#include "NetworkAPI.h"

#include "NetworkAPI_ProgressDialog.h"
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

static void addPart(QHttpMultiPart *form, const QString type, const QString disposition, const QString body)
{
	QHttpPart httpPart;
	httpPart.setRawHeader("Content-Type", type.toUtf8());
	httpPart.setRawHeader("Content-Disposition", disposition.toUtf8());
	httpPart.setBody(body.toUtf8());
	form->append(httpPart);
}

const QString NetworkAPI::FORM_BOUNDARY = QString("--%1_Boundary_%1--").arg(QRandomGenerator::global()->generate64(), 16, 16, QLatin1Char('0'));

NetworkAPI::NetworkAPI(const QUrl serverURL)
	: serverURL(serverURL)
{
	;
}

NetworkAPI::~NetworkAPI()
{
	;
}

QPair<int, QJsonArray> NetworkAPI::fetchRemoteToDos(QHttpMultiPart *form)
{
	qDebug() << "NetworkAPI::fetchRemoteToDos() Starts";
	// Send the request
	QNetworkRequest request(serverURL.resolved(QUrl("homework.php")));
	QNetworkReply *reply = post(request, form);
	// Set up the event loop & wait for the reply
	QEventLoop waitUntilFinished;
	QObject::connect(this, &QNetworkAccessManager::finished, &waitUntilFinished, &QEventLoop::quit);
	waitUntilFinished.exec();
	// Collect the required information
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	QJsonArray responseJSON = QJsonDocument::fromJson(reply->readAll()).array();
	qDebug() << statusCode << responseJSON.size();
	for (int i = 0; i < responseJSON.size(); ++i)
		qDebug() << responseJSON.at(i)["title"].toString() << responseJSON.at(i)["deadline"].toString() << responseJSON.at(i)["directory"].toString() << responseJSON.at(i)["checked"].toString();
	delete reply;
	qDebug() << "NetworkAPI::fetchRemoteToDos() Ends" << endl;
	return qMakePair(statusCode, responseJSON);
}

int NetworkAPI::uploadHomework(QHttpMultiPart *form)
{
	qDebug() << "NetworkAPI::uploadHomework() Starts";
	// Send the request
	QNetworkRequest request(serverURL.resolved(QUrl("upload.php")));
	request.setRawHeader("Content-Type", ("multipart/form-data;boundary=" + FORM_BOUNDARY).toUtf8());
	QNetworkReply *reply = post(request, form);
	// Set up the progress dialog & wait for the reply
	NetworkAPI_ProgressDialog progress;
	QObject::connect(reply, &QNetworkReply::uploadProgress, &progress, &NetworkAPI_ProgressDialog::updateProgress);
	QObject::connect(this, &QNetworkAccessManager::finished, &progress, &NetworkAPI_ProgressDialog::close);
	progress.exec();
	// Collect the required information
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	QString replyMessage = reply->readAll();
	QMessageBox(QMessageBox::Icon::Information, QString("HTTP %1").arg(statusCode), replyMessage, QMessageBox::Ok).exec();
	qDebug() << statusCode << replyMessage;
	delete reply;
	qDebug() << "NetworkAPI::uploadHomework() Ends" << endl;
	return statusCode;
}

QPair<int, QJsonArray> NetworkAPI::fetchRemoteToDos(const Settings &settings)
{
	// Check if offline
	if (!isOnline())
	{
		QMessageBox(QMessageBox::Icon::Warning, "No Internet", "Your computer is offline now. Please check your connection and try agagin later. ", QMessageBox::Ok).exec();
		return qMakePair(0, QJsonArray());
	}
	// Fulfill the form
	QHttpMultiPart *form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	form->setBoundary(FORM_BOUNDARY.toUtf8());
	// Append the information
	addPart(form, "text/plain", "form-data; name=\"Name\"", settings.getName());
	addPart(form, "text/plain", "form-data; name=\"Number\"", settings.getNumber());
	// Send the request & return
	return NetworkAPI(settings.getServer()).fetchRemoteToDos(form);
}

int NetworkAPI::uploadHomework(const Settings &settings, const QString directory)
{
	// Check if offline
	if (!isOnline())
	{
		QMessageBox(QMessageBox::Icon::Warning, "No Internet", "Your computer is offline now. Please check your connection and try agagin later. ", QMessageBox::Ok).exec();
		return 0;
	}
	// Get the file
	QString filename = QFileDialog::getOpenFileName(nullptr, "Open the file");
	// If no file selected or cancelled
	if (filename.isEmpty())
		return -1;
	// Fulfill the form
	QHttpMultiPart *form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
	form->setBoundary(FORM_BOUNDARY.toUtf8());
	// Append the information
	addPart(form, "text/plain", "form-data; name=\"Name\"", settings.getName());
	addPart(form, "text/plain", "form-data; name=\"Number\"", settings.getNumber());
	addPart(form, "text/plain", "form-data; name=\"Directory\"", directory);
	// Append the homework file
	QFile homework(filename);
	homework.open(QFile::ReadOnly);
	QHttpPart filePart;
	filePart.setRawHeader("Content-Type", "application/octet-stream");
	filePart.setRawHeader("Content-Disposition", "form-data; name=\"File\"; filename=\"" + homework.fileName().toUtf8() + "\"");
	filePart.setBodyDevice(&homework);
	form->append(filePart);
	// Send the request & return
	int result = NetworkAPI(settings.getServer()).uploadHomework(form);
	homework.close();
	return result;
}

bool NetworkAPI::isOnline()
{
	return QNetworkConfigurationManager().isOnline();
}
