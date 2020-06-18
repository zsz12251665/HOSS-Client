#include "RemoteAPI_ProgressDialog.h"
#include "ui_RemoteAPI_ProgressDialog.h"

RemoteAPI_ProgressDialog::RemoteAPI_ProgressDialog(QWidget *parent)
	: QDialog(parent), ui(new Ui::RemoteAPI_ProgressDialog)
{
	ui->setupUi(this);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

RemoteAPI_ProgressDialog::~RemoteAPI_ProgressDialog()
{
	delete ui;
}

void RemoteAPI_ProgressDialog::updateProgress(const int currentValue, const int maximumValue)
{
	ui->progressBar->setMaximum(maximumValue);
	ui->progressBar->setValue(currentValue);
}
