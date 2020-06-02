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
<<<<<<< HEAD
    QUrl url("http://47.112.198.206/homework.php");
    QJsonArray json = fetchRemoteToDos(url);
=======
	QUrl url("http://47.112.198.206/homework.php");
	QJsonArray json = QJsonArray(fetchRemoteToDos(url));
>>>>>>> f63b8530ff992eab1ad8af61a0cda10c2630a04e
    qDebug() << json.size();


}

void Homework::on_upload_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open the file"), "C:/", tr("All files(*.*)"));

    if (!fileName.isNull())
    {
           // qDebug() << fileName; // "C:/Users/翁浩瀚/Desktop/QQ截图20200601091029.png"
        QUrl url("http://47.112.198.206");
        QFile file(fileName);
        QString reply = uploadHomework(url, QString("翁浩瀚"), QString("201930251436"),
                               QString("第十四次大物作业"), &file);
        qDebug() << reply;
    }

}
