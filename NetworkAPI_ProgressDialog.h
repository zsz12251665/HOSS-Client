#ifndef NETWORKAPI_PROGRESSDIALOG_H
#define NETWORKAPI_PROGRESSDIALOG_H

#include <QDialog>

namespace Ui
{
	class NetworkAPI_ProgressDialog;
}

class NetworkAPI_ProgressDialog : public QDialog
{
	Q_OBJECT
private:
	Ui::NetworkAPI_ProgressDialog *ui;
public:
	NetworkAPI_ProgressDialog(QWidget* = nullptr);
	~NetworkAPI_ProgressDialog();
public slots:
	void updateProgress(int, int);
};

#endif // NETWORKAPI_PROGRESSDIALOG_H
