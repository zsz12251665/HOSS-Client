#ifndef REMOTEAPI_H
#define REMOTEAPI_H

#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QNetworkAccessManager>

class RemoteAPI
{
private:
	static const QString formBoundary;
	QNetworkAccessManager *manager;
	QEventLoop waitUntilFinished;
	QUrl serverURL;
public:
	RemoteAPI(const QUrl = QString("/"));
	~RemoteAPI();
	QJsonArray fetchRemoteToDos();
	int uploadHomework(const QString, const QString, const QString);
	int uploadHomework(const QString, const QString, const QString, QFile&);
};

#endif // REMOTEAPI_H
