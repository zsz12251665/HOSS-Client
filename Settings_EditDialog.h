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
	Settings_EditDialog(QString, QString, QString, QWidget* = nullptr);
	~Settings_EditDialog();
	QString getServer() const;
	QString getName() const;
	QString getNumber() const;
};

#endif // SETTINGS_EDITDIALOG_H
