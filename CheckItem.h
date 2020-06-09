#ifndef CHECKITEM_H
#define CHECKITEM_H

#include <QDate>
#include <QWidget>

namespace Ui
{
	class CheckItem;
}

class CheckItem : public QWidget
{
	Q_OBJECT
private:
	Ui::CheckItem *ui;
	int id;
	QString title, directory;
	QDate deadline;
	bool isFinished;
private slots:
	void enterEvent(QEvent*);
	void leaveEvent(QEvent*);
	void mouseDoubleClickEvent(QMouseEvent*);
	void on_button_check_clicked();
public:
	CheckItem(const int, const QString = "标题 TITLE HERE", const QDate = QDate::currentDate(),
			  const QString = QString(), const bool = false, QWidget* = nullptr);
	~CheckItem();
	int getId();
	QString getTitle();
	QDate getDeadline();
	bool getIsFinished();
	QString getDirectory();
	bool isRemote();
	bool isDeleted();
public slots:
	void deleteItem();
signals:
	void editEvent(CheckItem*);
};

#endif // CHECKITEM_H
