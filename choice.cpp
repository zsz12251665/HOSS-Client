#include "choice.h"
#include "ui_choice.h"
#include "remoteAPI.h"
#include "airplanner.h"
#include "homework.h"

Choice::Choice(QWidget *parent) :
  QMainWindow(parent),
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
	Airplanner *airplanner = new Airplanner;
	airplanner->show();
	this->close();
}

void Choice::on_pushButton_2_clicked()
{
	Homework *homework = new Homework;
	homework->show();
	this->close();
}
