#include "NetworkAPI_ProgressDialog.h"
#include "ui_NetworkAPI_ProgressDialog.h"

NetworkAPI_ProgressDialog::NetworkAPI_ProgressDialog(QWidget *parent)
	: QDialog(parent), ui(new Ui::NetworkAPI_ProgressDialog)
{
	ui->setupUi(this);
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint);
}

NetworkAPI_ProgressDialog::~NetworkAPI_ProgressDialog()
{
	delete ui;
}

void NetworkAPI_ProgressDialog::updateProgress(const int currentValue, const int maximumValue)
{
	ui->progressBar->setMaximum(maximumValue);
	ui->progressBar->setValue(currentValue);
}
