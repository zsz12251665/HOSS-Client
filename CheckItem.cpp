#include "CheckItem.h"
#include "ui_CheckItem.h"

#include "CheckItem_EditDialog.h"
#include "RemoteAPI.h"
#include "Settings.h"

#include <QDebug>

CheckItem::CheckItem(const int id, const QString title, const QDate deadline, const QString directory, const bool checked, QWidget *parent)
	: QWidget(parent), ui(new Ui::CheckItem), id(id), title(title), directory(directory), deadline(deadline), checked(checked)
{
	ui->setupUi(this);
	setTitle(title);
	setDeadline(deadline);
	setChecked(checked);
	setAutoFillBackground(true);
	// Only local homeworks could be deleted
	if (!isRemote())
		connect(ui->button_delete, &QPushButton::clicked, this, &CheckItem::remove);
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
	setPalette(QPalette());
	qDebug() << title << "mouse out!" << endl;
}

void CheckItem::mouseDoubleClickEvent(QMouseEvent*)
{
	if(!isRemote())
	{
		CheckItem_EditDialog editDialog(title, deadline);
		if (editDialog.exec() == QDialog::Accepted)
		{
			setTitle(editDialog.getTitle());
			setDeadline(editDialog.getDeadline());
			qDebug() << title << deadline;
			emit editEvent(this);
		}
	}
}

void CheckItem::on_button_check_clicked()
{
	if (isRemote())
	{
		int result = RemoteAPI::uploadHomework(Settings(), directory);
		if (result != -1)
			setChecked(result == 200);
	}
	else
		setChecked(!checked);
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

QString CheckItem::getDirectory() const
{
	return directory;
}

CheckItem::ShowState CheckItem::getShowState() const
{
	// Prevent deleted items, out-of-date remote items and unavailable remote items from showing
	if (isDeleted() || (isRemote() && (getDeadline() < QDate::currentDate() || !RemoteAPI::isOnline())))
		return ShowState::NONE;
	return isRemote() ? ShowState::REMOTE : ShowState::LOCAL;
}

bool CheckItem::isFinished() const
{
	return checked;
}

bool CheckItem::isRemote() const
{
	return !directory.isEmpty();
}

bool CheckItem::isDeleted() const
{
	return id < 0;
}

void CheckItem::setTitle(const QString value)
{
	if (title != value)
	{
		title = value;
		emit editEvent(this);
	}
	ui->label_title->setText(title);
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
