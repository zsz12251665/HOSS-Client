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

void Homework::on_input_task_returnPressed()
{
    if (ui->input_task->text() != "")
    {
        QString name = ui->input_task->text();
        QDate ddl = ui->dateEdit->date();
		CheckItem *item = new CheckItem(name, ddl, false);
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
        QSettings setting("setting.ini",QSettings::IniFormat);
        QString StuName = setting.value("config/StuName").toString();
        QString StuNumber = setting.value("config/StuNumber").toString();
        QFile file(fileName);
        QString reply = remoteAPI.uploadHomework(StuName, StuNumber, "test", file);
        qDebug() << reply;
    }
}

void Homework::on_Setting_clicked()
{
    // declare the setting
    QSettings setting("setting.ini",QSettings::IniFormat);

    // generate a multiply input dialog
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("Setting:"));

    QString value1 = QString("Server IP: ");
    QLineEdit *ip_input = new QLineEdit(&dialog);
    if(setting.contains("config/ip"))
    {
        ip_input->setText(setting.value("config/ip").toString());
    }
    form.addRow(value1, ip_input);

    QString value2 = QString("Student Name: ");
    QLineEdit *name_input = new QLineEdit(&dialog);
    if(setting.contains("config/StuName"))
    {
        name_input->setText(setting.value("config/StuName").toString());
    }
    form.addRow(value2, name_input);

    QString value3 = QString("Student Number: ");
    QLineEdit *number_input = new QLineEdit(&dialog);
    if(setting.contains("config/StuNumber"))
    {
        number_input->setText(setting.value("config/StuNumber").toString());
    }
    form.addRow(value3, number_input);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process if OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        setting.setValue("config/ip",ip_input->text());
        setting.setValue("config/StuName",name_input->text());
        setting.setValue("config/StuNumber",number_input->text());
        setting.sync();
    }
}
