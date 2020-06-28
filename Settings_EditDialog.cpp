#include "Settings_EditDialog.h"
#include "ui_Settings_EditDialog.h"

#include <QFileDialog>

Settings_EditDialog::Settings_EditDialog(const QString server, const QString name, const QString number, const QString background, QWidget *parent)
	: QDialog(parent), ui(new Ui::Settings_EditDialog)
{
	ui->setupUi(this);
	ui->edit_server->setText(server);
	ui->edit_name->setText(name);
	ui->edit_number->setText(number);
	ui->edit_background->setText(background);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

Settings_EditDialog::~Settings_EditDialog()
{
	delete ui;
}

void Settings_EditDialog::on_button_background_clicked()
{
	// Get the background picture from local file
	QString imageURL = QFileDialog::getOpenFileName(this, "Choose the background picture", ".", "Image (*.png *.jpg *.bmp)");
	if (imageURL.isEmpty())
		return;
	// Set the background path
	ui->edit_background->setText(QString("url(\"%1\")").arg(imageURL));
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

QString Settings_EditDialog::getBackground() const
{
	return ui->edit_background->text();
}
