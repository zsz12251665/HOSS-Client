#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings
{
private:
	QSettings config;
public:
	Settings(QString = "settings");
	~Settings();
	QString getServer() const;
	QString getName() const;
	QString getNumber() const;
	QVariant getConfig(const QString) const;
	void setConfig(const QString, const QVariant);
	void popEditDialog();
};

#endif // SETTINGS_H
