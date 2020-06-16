#ifndef REMOTEAPI_H
#define REMOTEAPI_H

#include <QNetworkAccessManager>

class Settings;

class RemoteAPI
{
private:
	static const QString formBoundary;
	QNetworkAccessManager *manager;
	QUrl serverURL;
	QPair<int, QJsonArray> fetchRemoteToDos(QHttpMultiPart*);
	int uploadHomework(QHttpMultiPart*);
public:
	RemoteAPI(const QUrl);
	~RemoteAPI();
	static QPair<int, QJsonArray> fetchRemoteToDos(const Settings&);
	static int uploadHomework(const Settings&, const QString);
	static bool isOnline();
};

#endif // REMOTEAPI_H
