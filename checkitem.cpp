#include "checkitem.h"
#include "ui_checkitem.h"

#include "CheckItem_EditDialog.h"

#include <QDebug>

CheckItem::CheckItem(QString name, QDate deadline, bool isRemote, QString directory,
					 QWidget *parent) :
	QWidget(parent), ui(new Ui::CheckItem),
	name(name), deadline(deadline), isRemote(isRemote), directory(directory)
{
	ui->setupUi(this);
	ui->label_title->setText(name);
	ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
	setAutoFillBackground(true);
//	if (isRemote)
//		connect(ui->checkBox,&QCheckBox::click, [this] {emit checkEvent(this);});
//	else
		connect(ui->button_delete, &QPushButton::clicked, [this] {emit removeEvent(this);});
}

CheckItem::~CheckItem()
{
	delete ui;
}

void CheckItem::enterEvent(QEvent*)
{
	static QPalette pal;
	pal.setColor(QPalette::Background,qRgb(135, 206, 250));
	setPalette(pal);
	qDebug() << name << " mouse in! isRemote: " << isRemote << endl;
}


void CheckItem::leaveEvent(QEvent*)
{
	setPalette(QPalette());
	qDebug() << name << " mouse out! " << endl;
}

void CheckItem::mouseDoubleClickEvent(QMouseEvent*)
{
	if(!isRemote)
	{
		CheckItem_EditDialog editDialog(name, deadline);
		if (editDialog.exec() == QDialog::Accepted)
		{
			name = editDialog.titleValue();
			deadline = editDialog.deadlineValue();
			qDebug() << name << " " << deadline << endl;
			ui->label_title->setText(name);
			ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
			emit editEvent(this);
		}
	}
}

QString CheckItem::getName()
{
	return this->name;
}
QDate CheckItem::getDeadline()
{
	return this->deadline;
}
bool CheckItem::getIsRemote()
{
	return this->isRemote;
}
QString CheckItem::getDirectory()
{
	return this->directory;
}
