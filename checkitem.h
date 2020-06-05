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
		bool isRemote;
		QString directory;
		void enterEvent(QEvent *) override;
		void leaveEvent(QEvent *) override;
		void mouseDoubleClickEvent(QMouseEvent *) override;
	public:
		CheckItem(QString, QDate, bool = false, QString = QString(), QWidget* = nullptr);
		~CheckItem();
		QString getName();
		QDate getDdl();
		bool getIsRemote();
		QString getDirectory();
	signals:
		void check_click(CheckItem*);
		void edit_click();
//	private slots:
//		void on_Edit_button_clicked();
};

#endif // CHECKITEM_H
