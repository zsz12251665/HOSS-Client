#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : private QSettings
{
public:
	Settings(QString = "settings");
	~Settings();
	QString getServer() const;
	QString getName() const;
	QString getNumber() const;
	QString getBackground() const;
	QString getLanguage() const;
	bool popEditDialog();
};

#endif // SETTINGS_H
