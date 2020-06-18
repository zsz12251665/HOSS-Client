#ifndef CHECKITEM_EDITDIALOG_H
#define CHECKITEM_EDITDIALOG_H

#include <QDate>
#include <QDialog>

namespace Ui
{
	class CheckItem_EditDialog;
}

class CheckItem_EditDialog : public QDialog
{
	Q_OBJECT
private:
	Ui::CheckItem_EditDialog *ui;
public:
	CheckItem_EditDialog(QString = QString(), QDate = QDate::currentDate(), QWidget* = nullptr);
	~CheckItem_EditDialog();
	QString getTitle() const;
	QDate getDeadline() const;
};

#endif // CHECKITEM_EDITDIALOG_H
