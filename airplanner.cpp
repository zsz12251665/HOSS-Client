#include "airplanner.h"
#include "ui_airplanner.h"
#include "QDateTime"

Airplanner::Airplanner(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Airplanner)
{
    ui->setupUi(this);

    ui->listWidget->setSpacing(6);
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QPixmap pixmap(":/assets/logo.ico");
    ui->MenuBar->setPixmap(pixmap.scaled(35, 35));

    ui->SubmitButton->setIcon(QIcon(":/new/icon/resource/edit.png"));
    ui->SubmitButton->setIconSize(QSize(26, 26));
    ui->RemoveButton->setIcon(QIcon(":/new/icon/resource/delete.png"));
    ui->RemoveButton->setIconSize(QSize(26, 26));
    ui->ExitButton->setIcon(QIcon(":/new/icon/resource/exit.png"));
    ui->ExitButton->setIconSize(QSize(25, 25));

    connect(ui->SubmitButton, SIGNAL (pressed()), this, SLOT (submitButton()));
    connect(ui->RemoveButton, SIGNAL (pressed()), this, SLOT (removeButton()));
    connect(ui->ExitButton, SIGNAL (pressed()), this, SLOT (exitButton()));
}

Airplanner::~Airplanner()
{
    delete ui;
}

void Airplanner::submitButton()
{
    if (ui->textEdit->toPlainText() != "")
    {
        QDateTime time = QDateTime::currentDateTime();
        ui->listWidget->addItem("\n" + ui->textEdit->toPlainText() + "\n\n\t\t" + time.toString("hh:mm dd.MM.yyyy"));
        ui->textEdit->clear();
    }
}

void Airplanner::removeButton()
{
    QListWidgetItem *item = ui->listWidget->takeItem(ui->listWidget->currentRow());
    delete item;
}

void Airplanner::exitButton()
{
    this->close();
}
