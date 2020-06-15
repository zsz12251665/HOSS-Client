#include "CheckItem_EditDialog.h"
#include "ui_CheckItem_EditDialog.h"

CheckItem_EditDialog::CheckItem_EditDialog(const QString title, const QDate deadline,
										   QWidget *parent) :
	QDialog(parent), ui(new Ui::CheckItem_EditDialog)
{
	ui->setupUi(this);
	ui->edit_title->setText(title);
	ui->edit_deadline->setDate(deadline);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
}

CheckItem_EditDialog::~CheckItem_EditDialog()
{
	delete ui;
}

QString CheckItem_EditDialog::getTitle() const
{
	return ui->edit_title->text();
}

QDate CheckItem_EditDialog::getDeadline() const
{
	return ui->edit_deadline->date();
}
