#ifndef CHECKITEM_H
#define CHECKITEM_H

#include <QDate>
#include <QDebug>
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
	QString name;
	QDate deadline;
	bool isRemote;
	QString directory;
private slots:
	void enterEvent(QEvent*);
	void leaveEvent(QEvent*);
	void mouseDoubleClickEvent(QMouseEvent*);
public:
	CheckItem(QString, QDate, bool = false, QString = QString(), QWidget* = nullptr);
	~CheckItem();
	QString getName();
	QDate getDdl();
	bool getIsRemote();
	QString getDirectory();
signals:
	void checkEvent(CheckItem*);
	void editEvent(CheckItem*);
	void removeEvent(CheckItem*);
};

#endif // CHECKITEM_H
