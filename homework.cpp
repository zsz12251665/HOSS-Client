#include "homework.h"
#include "ui_homework.h"
#include <QDate>
#include <QDebug>
#include <QMainWindow>

RemoteAPI remoteAPI(QString("http://47.112.198.206:8000/"));

Homework::Homework(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Homework),
    homework_list()
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDateTime::currentDateTime().date());
}

Homework::~Homework()
{
    delete ui;
}

void Homework::on_Add_button_clicked()
{
    if (ui->input_task->text() != "")
    {
        QString name = ui->input_task->text();
        QDate ddl = ui->dateEdit->date();
        CheckItem *item = new CheckItem(name, ddl);
        this->homework_list.append(item);
        ui->task_layout->addWidget(item);
        connect(item, &CheckItem::check_click, this, &Homework::removeTask);
        // qDebug() << name << ddl <<  this->homework_list.isEmpty();
        ui->input_task->clear();
    }
}

void Homework::on_input_task_returnPressed()
{
    if (ui->input_task->text() != "")
    {
        QString name = ui->input_task->text();
        QDate ddl = ui->dateEdit->date();
        CheckItem *item = new CheckItem(name, ddl);
        this->homework_list.append(item);
        ui->task_layout->addWidget(item);
        connect(item, &CheckItem::check_click, this, &Homework::removeTask);
        // qDebug() << name << ddl <<  this->homework_list.isEmpty();
        ui->input_task->clear();
    }
}

void Homework::removeTask(CheckItem *item)
{
    homework_list.removeOne(item);
    ui->task_layout->removeWidget(item);
    item->setParent(0);
    delete item;
}

void Homework::on_update_clicked()
{
	QJsonArray json = QJsonArray(remoteAPI.fetchRemoteToDos());
    qDebug() << json.size();
}

void Homework::on_upload_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
			tr("Open the file"), "C:/", tr("All files(*.*)"));

    if (!fileName.isNull())
	{
        QFile file(fileName);
		QString reply = remoteAPI.uploadHomework("翁浩瀚", "201930251436", "test", file);
        qDebug() << reply;
    }

}
