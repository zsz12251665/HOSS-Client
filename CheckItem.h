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
	CheckItem(const int, const QString, const QDate,
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
	void selfDelete();
signals:
	void editEvent(CheckItem*);
};

#endif // CHECKITEM_H
