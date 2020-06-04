#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QEventLoop>
#include <QFile>
#include <QPair>
#include <QRandomGenerator>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QHttpPart>
#include <QtNetwork/QHttpMultiPart>

#include <QJsonDocument>
#include <QJsonArray>

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
		QPair<int, QByteArray> uploadHomework(const QString, const QString,
											  const QString, QFile&);
};

#endif // NETWORK_H
