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
	QJsonArray fetchRemoteToDos();
	int uploadHomework(const QString, const QString, const QString, const QString);
	int verifySubmission(const QString, const QString, const QString);
public:
	RemoteAPI(const QUrl);
	~RemoteAPI();
	static QJsonArray fetchRemoteToDos(const Settings&);
	static int uploadHomework(const Settings&, const QString);
	static int verifySubmission(const Settings&, const QString);
};

#endif // REMOTEAPI_H
