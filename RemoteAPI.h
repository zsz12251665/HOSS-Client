#ifndef REMOTEAPI_H
#define REMOTEAPI_H

#include <QEventLoop>
#include <QFile>
#include <QHttpPart>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QPair>
// #include <QObject> // Included in other head files

class RemoteAPI : public QObject
{
	Q_OBJECT
private:
	static QString formBoundary;
	QNetworkAccessManager *manager;
	QEventLoop waitUntilFinished;
	QUrl serverURL;
	static QHttpPart makePart(const QString, const QString, const QString = QString());
public:
	RemoteAPI(const QUrl = QString("/"));
	~RemoteAPI();
	QJsonArray fetchRemoteToDos();
	QPair<int, QByteArray> uploadHomework(const QString, const QString, const QString, QFile&);
};

#endif // REMOTEAPI_H
