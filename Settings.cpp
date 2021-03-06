#include "Settings.h"

#include "Settings_EditDialog.h"

#include <QDebug>

Settings::Settings(const QString filename)
	: QSettings(filename + ".ini", QSettings::IniFormat)
{
	;
}

Settings::~Settings()
{
	sync();
}

QString Settings::getServer() const
{
	return value("config/server").toString();
}

QString Settings::getName() const
{
	return value("config/name").toString();
}

QString Settings::getNumber() const
{
	return value("config/number").toString();
}

QString Settings::getBackground() const
{
	return value("config/background").toString();
}

QString Settings::getLanguage() const
{
	return value("config/language", "ENG").toString();
}

bool Settings::popEditDialog()
{
	qDebug() << "Settings::popEditDialog() Starts";
	Settings_EditDialog editDialog(getServer(), getName(), getNumber(), getBackground(), getLanguage());
	// Keep popping until the input is valid or cancelled
	for (int result = editDialog.exec(); result != QDialog::Accepted || editDialog.getServer().isEmpty(); result = editDialog.exec())
	{
		// Handle cancel event
		if (result == QDialog::Rejected)
		{
			qDebug() << "Settings::popEditDialog() Ends" << endl;
			return false;
		}
	}
	// Update the configuration
	setValue("config/server", editDialog.getServer());
	setValue("config/name", editDialog.getName());
	setValue("config/number", editDialog.getNumber());
	setValue("config/background", editDialog.getBackground());
	setValue("config/language", editDialog.getLanguage());
	sync();
	qDebug() << getServer() << getName() << getNumber() << getBackground() << getLanguage();
	qDebug() << "Settings::popEditDialog() Ends" << endl;
	return true;
}
