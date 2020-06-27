#include "Settings_EditDialog.h"
#include "ui_Settings_EditDialog.h"

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

void Settings_EditDialog::on_change_background_clicked()
{
    // get the background picture form local file
    QString backgroundPic = QFileDialog::getOpenFileName(this, "choose the background picture", ".", "Text(*.png *.jpg *.bmp)");
    if (backgroundPic.isEmpty()) {
          QMessageBox mesg;
          mesg.warning(this, "warn", "Fail to get the background picture!");
          return;
    }
    // set the background path
    ui->edit_background->setText(backgroundPic);
}

QString Settings_EditDialog::getBackground() const
{
    return ui->edit_background->text();
}
