#include "homework.h"
#include "ui_homework.h"
#include <QDate>
#include <QDebug>
#include <QMainWindow>

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
    QUrl url("http://47.112.198.206/");
    QJsonArray json = fetchRemoteToDos(url);
    qDebug() << json.size();


}
