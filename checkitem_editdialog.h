#ifndef CHECKITEM_EDITDIALOG_H
#define CHECKITEM_EDITDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
	class CheckItem_EditDialog;
}

class CheckItem_EditDialog : public QDialog
{
	Q_OBJECT
	private:
		Ui::CheckItem_EditDialog *ui;
	public:
		CheckItem_EditDialog(const QString = QString(), const QDate = QDate(),
							 QWidget* = nullptr);
		~CheckItem_EditDialog();
		QString titleValue();
		QDate deadlineValue();
};

#endif // CHECKITEM_EDITDIALOG_H