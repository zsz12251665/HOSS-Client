#include "homework.h"
#include "ui_homework.h"
#include <QDate>

Homework::Homework(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Homework)
{
    ui->setupUi(this);

    QPixmap pixmap(":/assets/logo.ico");

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
        ui->listWidget->addItem("\n" + ui->input_task->text() + "\n\n\t\t" + ui->dateEdit->date().toString("dd.MM.yyyy"));
        ui->input_task->clear();
    }
}

void Homework::on_input_task_returnPressed()
{
    if (ui->input_task->text() != "")
    {
        ui->listWidget->addItem("\n" + ui->input_task->text() + "\n\n\t\t" + ui->dateEdit->date().toString("dd.MM.yyyy"));
        ui->input_task->clear();
    }
}
