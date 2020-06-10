#include "Settings.h"

#include "Settings_EditDialog.h"

Settings::Settings(QString filename) : config(filename + ".ini", QSettings::IniFormat)
{
	;
}

Settings::~Settings()
{
	config.sync();
}

QString Settings::getServer()
{
	return getConfig("server").toString();
}

QString Settings::getName()
{
	return getConfig("name").toString();
}

QString Settings::getNumber()
{
	return getConfig("number").toString();
}

QVariant Settings::getConfig(const QString target)
{
	return config.value("config/" + target);
}

void Settings::setConfig(const QString target, const QVariant value)
{
	config.setValue("config/" + target, value);
	config.sync();
}

void Settings::popEditDialog()
{
	Settings_EditDialog editDialog(getServer(), getName(), getNumber());
	do
		// Process if OK button is clicked
		if (editDialog.exec() == QDialog::Accepted)
		{
			config.setValue("config/server", editDialog.serverValue());
			config.setValue("config/name", editDialog.nameValue());
			config.setValue("config/number", editDialog.numberValue());
			config.sync();
		}
	// Server URL must be set
	while (getServer().isEmpty());
}
