#include "CheckItem.h"
#include "ui_CheckItem.h"

#include "CheckItem_EditDialog.h"
#include "RemoteAPI.h"
#include "Settings.h"

#include <QDebug>

static inline QIcon getIcon(bool checked)
{
	QString filename = checked ? "check-circle.svg" : "circle.svg";
	return QIcon(":/FontAwesome/svgs/regular/" + filename);
}

CheckItem::CheckItem(const int id, const QString title, const QDate deadline,
					 const QString directory, const bool isFinished, QWidget *parent) :
	QWidget(parent), ui(new Ui::CheckItem), id(id), title(title), directory(directory),
	deadline(deadline), checked(isFinished)
{
	ui->setupUi(this);
	ui->label_title->setText(title);
	ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
	ui->button_check->setIcon(getIcon(checked));
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
	static QPalette pal;
	pal.setColor(QPalette::Background, QColor(135, 206, 250, 127));
	setPalette(pal);
	qDebug() << title << "mouse in! isRemote:" << isRemote();
}


void CheckItem::leaveEvent(QEvent*)
{
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
			title = editDialog.getTitle();
			deadline = editDialog.getDeadline();
			qDebug() << title << deadline;
			ui->label_title->setText(title);
			ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
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
	// Prevent deleted, out-of-date remote items and unavailable remote items from showing
	if (isDeleted() || (isRemote() && (getDeadline() < QDate::currentDate() ||
									   !RemoteAPI::isOnline())))
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

void CheckItem::setChecked(bool isFinished)
{
	if (checked != isFinished)
	{
		checked = isFinished;
		ui->button_check->setIcon(getIcon(checked));
		emit editEvent(this);
	}
}

void CheckItem::remove()
{
	this->hide();
	id = ~id;
	emit editEvent(this);
}
