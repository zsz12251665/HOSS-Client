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
protected:
	Ui::CheckItem *ui;
private:
	int id;
	QString title;
	QDate deadline;
	bool checked;
private slots:
	void enterEvent(QEvent*);
	void leaveEvent(QEvent*);
	virtual void mouseDoubleClickEvent(QMouseEvent*);
	virtual void on_button_check_clicked() = 0;
public:
	enum ShowState
	{
		NONE = 0,	// 00
		LOCAL = 1,	// 01
		REMOTE = 2,	// 10
		ALL = 3		// 11
	};
	CheckItem(int, QString, QDate, bool = false, QWidget* = nullptr);
	~CheckItem();
	int getId() const;
	QString getTitle() const;
	QDate getDeadline() const;
	virtual ShowState getShowState() const = 0;
	bool isDeleted() const;
	bool isExpired() const;
	bool isFinished() const;
	virtual bool isRemote() const = 0;
	void setChecked(bool);
protected:
	void setDeadline(QDate);
	void setTitle(QString);
public slots:
	void remove();
signals:
	void editEvent(CheckItem*);
};

class LocalCheckItem : public CheckItem
{
	Q_OBJECT
private slots:
	void mouseDoubleClickEvent(QMouseEvent*) override;
	void on_button_check_clicked() override;
public:
	LocalCheckItem(int, QString, QDate, bool = false, QWidget* = nullptr);
	~LocalCheckItem();
	ShowState getShowState() const override;
	bool isRemote() const override;
};

class RemoteCheckItem : public CheckItem
{
	Q_OBJECT
private:
	QString directory;
private slots:
	void on_button_check_clicked() override;
public:
	RemoteCheckItem(int, QString, QDate, QString , bool = false, QWidget* = nullptr);
	~RemoteCheckItem();
	QString getDirectory() const;
	ShowState getShowState() const override;
	bool isRemote() const override;
	bool isSame(QJsonValue) const;
};


#endif // CHECKITEM_H
