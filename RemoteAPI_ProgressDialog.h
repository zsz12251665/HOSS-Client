#ifndef REMOTEAPI_PROGRESSDIALOG_H
#define REMOTEAPI_PROGRESSDIALOG_H

#include <QDialog>

namespace Ui
{
	class RemoteAPI_ProgressDialog;
}

class RemoteAPI_ProgressDialog : public QDialog
{
	Q_OBJECT
private:
	Ui::RemoteAPI_ProgressDialog *ui;
public:
	RemoteAPI_ProgressDialog(QWidget* = nullptr);
	~RemoteAPI_ProgressDialog();
public slots:
	void updateProgress(int, int);
};

#endif // REMOTEAPI_PROGRESSDIALOG_H
