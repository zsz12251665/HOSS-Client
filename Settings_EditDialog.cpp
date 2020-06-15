#include "Settings_EditDialog.h"
#include "ui_Settings_EditDialog.h"

Settings_EditDialog::Settings_EditDialog(const QString server, const QString name,
										 const QString number, QWidget *parent) :
	QDialog(parent), ui(new Ui::Settings_EditDialog)
{
	ui->setupUi(this);
	ui->edit_server->setText(server);
	ui->edit_name->setText(name);
	ui->edit_number->setText(number);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

Settings_EditDialog::~Settings_EditDialog()
{
	delete ui;
}

QString Settings_EditDialog::getServer() const
{
	return ui->edit_server->text();
}

QString Settings_EditDialog::getName() const
{
	return ui->edit_name->text();
}

QString Settings_EditDialog::getNumber() const
{
	return ui->edit_number->text();
}
