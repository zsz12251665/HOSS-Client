#ifndef SETTINGS_H
#define SETTINGS_H

#include<QSettings>

class Settings
{
private:
	QSettings config;
public:
	Settings(QString = "settings");
	~Settings();
	QString getServer();
	QString getName();
	QString getNumber();
	QVariant getConfig(const QString);
	void setConfig(const QString, const QVariant);
	void popEditDialog();
};

#endif // SETTINGS_H
