#include "CheckItem.h"
#include "ui_CheckItem.h"

#include "CheckItem_EditDialog.h"
#include "NetworkAPI.h"
#include "Settings.h"

#include <QDebug>
#include <QJsonValue>

CheckItem::CheckItem(const int id, const QString title, const QDate deadline, const bool checked, QWidget *parent)
	: QWidget(parent), ui(new Ui::CheckItem), id(id), title(title), deadline(deadline), checked(checked)
{
	ui->setupUi(this);
	setTitle(title);
	setDeadline(deadline);
	setChecked(checked);
	setAutoFillBackground(true);
    setPalette(QColor(244,244,244,2));
}

CheckItem::~CheckItem()
{
	delete ui;
}

void CheckItem::enterEvent(QEvent*)
{
	// Set the background as light blue
	setPalette(QColor(135, 206, 250, 127));
	qDebug() << title << "mouse in! isRemote:" << isRemote();
}

void CheckItem::leaveEvent(QEvent*)
{
	// Restore the background
    setPalette(QColor(244,244,244,2));
	qDebug() << title << "mouse out!" << endl;
}

int CheckItem::getId() const
{
	return id < 0 ? ~id : id;
}

QString CheckItem::getTitle() const
{
	return title;
}

QDate CheckItem::getDeadline() const
{
	return deadline;
}

bool CheckItem::isDeleted() const
{
	return id < 0;
}

bool CheckItem::isExpired() const
{
	return deadline < QDate::currentDate();
}

bool CheckItem::isFinished() const
{
	return checked;
}

void CheckItem::setTitle(const QString value)
{
	if (title != value)
	{
		title = value;
		emit editEvent(this);
	}
	ui->label_title->setText(title.isEmpty() ? "(No Title)" : title);
}

void CheckItem::setDeadline(const QDate value)
{
	if (deadline != value)
	{
		deadline = value;
		emit editEvent(this);
	}
	ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
}

void CheckItem::setChecked(const bool value)
{
	if (checked != value)
	{
		checked = value;
		emit editEvent(this);
	}
	ui->button_check->setIcon(QIcon(checked ? ":/FontAwesome/svgs/regular/check-circle.svg" : ":/FontAwesome/svgs/regular/circle.svg"));
}

void CheckItem::remove()
{
	this->hide();
	id = ~id;
	emit editEvent(this);
}

LocalItem::LocalItem(const int id, const QString title, const QDate deadline, const bool checked, QWidget *parent)
	: CheckItem(id, title, deadline, checked, parent)
{
	// Only local items could be deleted
	connect(ui->button_delete, &QPushButton::clicked, this, &CheckItem::remove);
}

LocalItem::~LocalItem()
{
	;
}

void LocalItem::mouseDoubleClickEvent(QMouseEvent*)
{
	// Only local items can be modified
	CheckItem_EditDialog editDialog(getTitle(), getDeadline());
	if (editDialog.exec() == QDialog::Accepted)
	{
		setTitle(editDialog.getTitle());
		setDeadline(editDialog.getDeadline());
		qDebug() << getTitle() << getDeadline();
		emit editEvent(this);
	}
}

void LocalItem::on_button_check_clicked()
{
	setChecked(!isFinished());
}

CheckItem::ShowState LocalItem::getShowState() const
{
	// Prevent deleted items from showing
	return isDeleted() ? ShowState::NONE : ShowState::LOCAL;
}

bool LocalItem::isRemote() const
{
	return false;
}

RemoteItem::RemoteItem(const int id, const QString title, const QDate deadline, const QString directory, const bool checked, QWidget *parent)
	: CheckItem(id, title, deadline, checked, parent), directory(directory)
{
	;
}

RemoteItem::~RemoteItem()
{
	;
}

void RemoteItem::on_button_check_clicked()
{
	int result = NetworkAPI::uploadHomework(Settings(), directory);
	if (result != -1)
		setChecked(result == 200);
}

QString RemoteItem::getDirectory() const
{
	return directory;
}

CheckItem::ShowState RemoteItem::getShowState() const
{
	// Prevent deleted items, expired items and unavailable items from showing
	return isDeleted() || isExpired() || !NetworkAPI::isOnline() ? ShowState::NONE : ShowState::REMOTE;
}

bool RemoteItem::isRemote() const
{
	return true;
}

bool RemoteItem::isSame(QJsonValue target) const
{
	return target["title"].toString() == getTitle() && target["deadline"].toVariant().toDate() == getDeadline() && target["directory"].toString() == getDirectory();
}
