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

bool Settings::popEditDialog()
{
	Settings_EditDialog editDialog(getServer(), getName(), getNumber(), getBackground());
	// Keep popping until the input is valid or cancelled
	for (int result = editDialog.exec(); result != QDialog::Accepted || editDialog.getServer().isEmpty(); result = editDialog.exec())
	{
		// Handle cancel event
		if (result == QDialog::Rejected)
			return false;
	}
	// Update the configuration
	setValue("config/server", editDialog.getServer());
	setValue("config/name", editDialog.getName());
	setValue("config/number", editDialog.getNumber());
	setValue("config/background", editDialog.getBackground());
	sync();
	return true;
}
