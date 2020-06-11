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
public:
	CheckItem(const int, const QString, const QDate,
			  const QString = QString(), const bool = false, QWidget* = nullptr);
	~CheckItem();
	int getId() const;
	QString getTitle() const;
	QDate getDeadline() const;
	bool getIsFinished() const;
	QString getDirectory() const;
	bool isRemote() const;
	bool isDeleted() const;
public slots:
	void on_button_check_clicked(const bool = true);
	void selfDelete();
signals:
	void editEvent(CheckItem*);
};

#endif // CHECKITEM_H
