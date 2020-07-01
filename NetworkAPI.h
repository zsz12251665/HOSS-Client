#ifndef NETWORKAPI_H
#define NETWORKAPI_H

#include <QNetworkAccessManager>

class Settings;

class NetworkAPI : private QNetworkAccessManager
{
private:
	static const QString FORM_BOUNDARY;
	QUrl serverURL;
	QPair<int, QJsonArray> fetchRemoteToDos(QHttpMultiPart*);
	int uploadHomework(QHttpMultiPart*);
public:
	NetworkAPI(QUrl);
	~NetworkAPI();
	static QPair<int, QJsonArray> fetchRemoteToDos(const Settings&);
	static int uploadHomework(const Settings&, QString);
	static bool isOnline();
};

#endif // NETWORKAPI_H
