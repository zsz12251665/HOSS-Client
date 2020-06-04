#ifndef CHECKITEM_H
#define CHECKITEM_H

#include <QWidget>
#include <QDate>
#include <QDateEdit>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>

namespace Ui {
class CheckItem;
}

class CheckItem : public QWidget
{
	Q_OBJECT
	private:
		Ui::CheckItem *ui;
		QString name;
		QDate deadline;
		bool fromRemote;
		void mouseDoubleClickEvent(QMouseEvent *);
	public:
		CheckItem(QString, QDate, bool, QWidget* = nullptr);
		~CheckItem();
	signals:
		void check_click(CheckItem*);
//	private slots:
//		void on_Edit_button_clicked();
};

#endif // CHECKITEM_H
