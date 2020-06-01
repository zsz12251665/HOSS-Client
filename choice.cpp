#include "choice.h"
#include "ui_choice.h"
#include "network.h"

Choice::Choice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Choice)
{
    ui->setupUi(this);
}

Choice::~Choice()
{
    delete ui;
}

void Choice::on_pushButton_clicked()
{
    accept();
}

void Choice::on_pushButton_2_clicked()
{
    reject();
}
