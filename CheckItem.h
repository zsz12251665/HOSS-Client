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
	bool checked;
private slots:
	void enterEvent(QEvent*);
	void leaveEvent(QEvent*);
	void mouseDoubleClickEvent(QMouseEvent*);
	void on_button_check_clicked();
public:
	enum ShowState
	{
		NONE = 0,	// 00
		LOCAL = 1,	// 01
		REMOTE = 2,	// 10
		ALL = 3		// 11
	};
	CheckItem(const int, const QString, const QDate,
			  const QString = QString(), const bool = false, QWidget* = nullptr);
	~CheckItem();
	int getId() const;
	QString getTitle() const;
	QDate getDeadline() const;
	QString getDirectory() const;
	ShowState getShowState() const;
	bool isFinished() const;
	bool isRemote() const;
	bool isDeleted() const;
	void setChecked(bool);
public slots:
	void remove();
signals:
	void editEvent(CheckItem*);
};

#endif // CHECKITEM_H
