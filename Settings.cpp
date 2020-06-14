#include "Settings.h"

#include "Settings_EditDialog.h"

#include <QDebug>

Settings::Settings(QString filename) : QSettings(filename + ".ini", QSettings::IniFormat)
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

bool Settings::popEditDialog()
{
	Settings_EditDialog editDialog(getServer(), getName(), getNumber());
	// Keep popping until the input is valid
	for (int result = editDialog.exec(); result != QDialog::Accepted ||
		 editDialog.serverValue().isEmpty(); result = editDialog.exec())
	{
		// Handle cancel event
		if (result == QDialog::Rejected)
			return false;
	}
	setValue("config/server", editDialog.serverValue());
	setValue("config/name", editDialog.nameValue());
	setValue("config/number", editDialog.numberValue());
	sync();
	return true;
}
