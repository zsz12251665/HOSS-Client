#include "checkitem.h"
#include "ui_checkitem.h"
#include "checkitem_editdialog.h"

CheckItem::CheckItem(QString name, QDate deadline, bool isRemote, QString directory, QWidget *parent) :
	QWidget(parent), ui(new Ui::CheckItem),
	name(name), deadline(deadline), isRemote(isRemote), directory(directory)
{
	ui->setupUi(this);
	ui->label_title->setText(name);
	ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
	connect(ui->button_delete, &QPushButton::clicked, [this] {emit check_click(this);});
}

CheckItem::~CheckItem()
{
    delete ui;
}

void CheckItem::mouseDoubleClickEvent(QMouseEvent *event)
{
	CheckItem_EditDialog editDialog(name, deadline);
	if (editDialog.exec() == QDialog::Accepted)
	{
		name = editDialog.titleValue();
		deadline = editDialog.deadlineValue();
		qDebug() << name << " " << deadline << endl;
		ui->label_title->setText(name);
		ui->label_deadline->setText(deadline.toString("yyyy-MM-dd"));
	}
}

QString CheckItem::getName()
{
    return this->name;
}
QDate CheckItem::getDdl()
{
    return this->ddl;
}
bool CheckItem::getIsRemote()
{
    return this->isRemote;
}
QString CheckItem::getDirectory()
{
    return this->directory;
}
