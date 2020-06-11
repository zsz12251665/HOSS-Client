#include "CheckItem.h"
#include "ui_CheckItem.h"

#include "CheckItem_EditDialog.h"
#include "RemoteAPI.h"
#include "Settings.h"

#include <QDebug>

static inline QIcon getIcon(bool isFinished)
{
	QString filename = isFinished ? "check-circle.svg" : "circle.svg";
	return QIcon(":/FontAwesome/svgs/regular/" + filename);
}

CheckItem::CheckItem(const int id, const QString title, const QDate deadline,
					 const QString directory, const bool isFinished, QWidget *parent) :
	QWidget(parent), ui(new Ui::CheckItem), id(id), title(title), directory(directory),
	deadline(deadline), isFinished(isFinished)
{
	ui->setupUi(this);
	ui->label_title->setText(title);
	ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
	if (isRemote())
		this->isFinished = RemoteAPI::verifySubmission(new Settings(), directory) == 200;
	ui->button_check->setIcon(getIcon(isFinished));
	setAutoFillBackground(true);
	// Remote homework could not be deleted
	if (!isRemote())
		connect(ui->button_delete, &QPushButton::clicked, this, &CheckItem::selfDelete);
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
			title = editDialog.titleValue();
			deadline = editDialog.deadlineValue();
			qDebug() << title << deadline;
			ui->label_title->setText(title);
			ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
			emit editEvent(this);
		}
	}
}

void CheckItem::on_button_check_clicked(const bool needVerify)
{
	if (isRemote())
	{
		int result = needVerify ? RemoteAPI::verifySubmission(new Settings(), directory) :
								  RemoteAPI::uploadHomework(new Settings(), directory);
		// Check state unchanged, no need to emit the edit event
		if (result == -1 || isFinished == (result == 200))
			return;
	}
	isFinished = !isFinished;
	ui->button_check->setIcon(getIcon(isFinished));
	emit editEvent(this);
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

bool CheckItem::getIsFinished() const
{
	return isFinished;
}

QString CheckItem::getDirectory() const
{
	return directory;
}

bool CheckItem::isRemote() const
{
	return !directory.isEmpty();
}

bool CheckItem::isDeleted() const
{
	return id < 0;
}

void CheckItem::selfDelete()
{
	this->hide();
	id = ~id;
	emit editEvent(this);
}
