#ifndef REMOTEAPI_H
#define REMOTEAPI_H

#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QPair>

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
	QPair<int, QByteArray> uploadHomework(const QString, const QString, const QString);
	QPair<int, QByteArray> uploadHomework(const QString, const QString, const QString, QFile&);
};

#endif // REMOTEAPI_H
