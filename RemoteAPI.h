#ifndef REMOTEAPI_H
#define REMOTEAPI_H

#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QNetworkAccessManager>

class Settings;

class RemoteAPI
{
private:
	static const QString formBoundary;
	QNetworkAccessManager *manager;
	QEventLoop waitUntilFinished;
	QUrl serverURL;
public:
	RemoteAPI(const QUrl);
	~RemoteAPI();
	QJsonArray fetchRemoteToDos();
	int uploadHomework(const QString, const QString, const QString, const QString);
	int verifySubmission(const QString, const QString, const QString);
	static int uploadHomework(const Settings&, const QString);
	static int verifySubmission(const Settings&, const QString);
};

#endif // REMOTEAPI_H
