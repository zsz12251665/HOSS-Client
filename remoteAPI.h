#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QEventLoop>
#include <QFile>
#include <QRandomGenerator>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpPart>
#include <QtNetwork/QHttpMultiPart>

#include <QJsonDocument>
#include <QJsonArray>

QJsonArray fetchRemoteToDos(QUrl remoteURL);
QString uploadHomework(QUrl remoteURL, QString name, QString number,
                       QString directory, QFile *homework);

class RemoteAPI : public QObject
{
	private:
		static QString formBoundary;
		QNetworkAccessManager *manager;
		QEventLoop waitUntilFinished;
		QUrl serverURL;
		static QHttpPart makePart(const QString, const QString, const QString = "");
	public:
		RemoteAPI(const QUrl = QString("/"));
		~RemoteAPI();
		QJsonArray fetchRemoteToDos();
		QString uploadHomework(const QString, const QString, const QString, QFile&);
};

#endif // NETWORK_H
