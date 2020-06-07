#ifndef NETWORK_H
#define NETWORK_H

#include <QEventLoop>
#include <QFile>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QPair>
#include <QRandomGenerator>

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

#endif // NETWORK_H
