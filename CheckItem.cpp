#include "CheckItem.h"
#include "ui_CheckItem.h"

#include "CheckItem_EditDialog.h"
#include "RemoteAPI.h"
#include "Settings.h"

#include <QDebug>

CheckItem::CheckItem(const int id, const QString title, const QDate deadline,
					 const QString directory, const bool isFinished, QWidget *parent) :
	QWidget(parent), ui(new Ui::CheckItem), id(id), title(title), directory(directory),
	deadline(deadline), isFinished(isFinished)
{
	ui->setupUi(this);
	ui->label_title->setText(title);
	ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
	ui->checkBox->setCheckState(Qt::CheckState(2*isFinished));
	setAutoFillBackground(true);
}

CheckItem::~CheckItem()
{
	delete ui;
}

void CheckItem::enterEvent(QEvent*)
{
	static QPalette pal;
	pal.setColor(QPalette::Background, qRgb(135, 206, 250));
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

void CheckItem::on_button_delete_clicked()
{
	// Remote todos cannot be deleted
	if (isRemote())
		return;
	this->hide();
	id = ~id;
	emit editEvent(this);
}

void CheckItem::on_checkBox_stateChanged(int checkState)
{
	if (!isRemote())
	{
		isFinished = checkState == Qt::CheckState::Checked;
		emit editEvent(this);
		return;
	}
	Settings settings;
	int result = RemoteAPI(settings.getServer()).uploadHomework(settings.getName(),
																settings.getNumber(),
																directory);
	ui->checkBox->setCheckState(result == 200 ? Qt::CheckState::Checked :
												Qt::CheckState::Unchecked);
	if (isFinished ^ (result == 200))
	{
		isFinished = result == 200;
		emit editEvent(this);
	}
}

int CheckItem::getId()
{
	return id < 0 ? ~id : id;
}

QString CheckItem::getTitle()
{
	return title;
}

QDate CheckItem::getDeadline()
{
	return deadline;
}

bool CheckItem::getIsFinished()
{
	return isFinished;
}

QString CheckItem::getDirectory()
{
	return directory;
}

bool CheckItem::isRemote()
{
	return !directory.isEmpty();
}

bool CheckItem::isDeleted()
{
	return id < 0;
}
