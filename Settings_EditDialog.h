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
protected:
	Ui::Settings_EditDialog *ui;
private slots:
	void on_button_background_clicked();
public:
	Settings_EditDialog(QString, QString, QString, QString, QWidget* = nullptr);
	~Settings_EditDialog();
	QString getServer() const;
	QString getName() const;
	QString getNumber() const;
	QString getBackground() const;
};

#endif // SETTINGS_EDITDIALOG_H
