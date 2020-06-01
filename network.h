#ifndef NETWORK_H
#define NETWORK_H

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

QJsonArray fetchRemoteToDos(QUrl remoteURL);
QString uploadHomework(QUrl remoteURL, QString name, QString number,
                       QString directory, QFile *homework);



#endif // NETWORK_H
