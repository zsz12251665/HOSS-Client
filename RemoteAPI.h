#ifndef REMOTEAPI_H
#define REMOTEAPI_H

#include <QNetworkAccessManager>

class Settings;

class RemoteAPI : private QNetworkAccessManager
{
private:
	static const QString formBoundary;
	QUrl serverURL;
	QPair<int, QJsonArray> fetchRemoteToDos(QHttpMultiPart*);
	int uploadHomework(QHttpMultiPart*);
public:
	RemoteAPI(QUrl);
	~RemoteAPI();
	static QPair<int, QJsonArray> fetchRemoteToDos(const Settings&);
	static int uploadHomework(const Settings&, QString);
	static bool isOnline();
};

#endif // REMOTEAPI_H
