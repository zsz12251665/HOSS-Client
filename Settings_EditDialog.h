#ifndef SETTINGS_EDITDIALOG_H
#define SETTINGS_EDITDIALOG_H

#include <QDialog>

namespace Ui
{
	class Settings_EditDialog;
}

class Settings_EditDialog : public QDialog
{
	Q_OBJECT
private:
	Ui::Settings_EditDialog *ui;
public:
	Settings_EditDialog(const QString, const QString, const QString, QWidget* = nullptr);
	~Settings_EditDialog();
	QString serverValue();
	QString nameValue();
	QString numberValue();
};

#endif // SETTINGS_EDITDIALOG_H
