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

    // get the remote todos
    QJsonArray json = QJsonArray(remoteAPI.fetchRemoteToDos());
    for (int i=0;i<json.size();i++) {
        qDebug() << json.at(i)["deadline"].toString();
        QString name = json.at(i)["title"].toString();
        QDate ddl = QDate::fromString(json.at(i)["deadline"].toString(), "yyyy-MM-dd");
        QString directory = json.at(i)["directory"].toString();
        CheckItem *item = new CheckItem(name, ddl, true, directory);
        this->homework_list.append(item);
    }

    // get local todos
    QSettings setting("setting.ini",QSettings::IniFormat);
    int len = setting.beginReadArray("todos");
    for(int i=0;i<len;i++)
    {
        setting.setArrayIndex(i);
        QString name = setting.value("name").toString();
        QDate ddl = QDate::fromString(setting.value("ddl").toString(), "dd.MM.yyyy");
        CheckItem *item = new CheckItem(name, ddl, true);
        this->homework_list.append(item);
    }
    setting.endArray();

    // add weigets
    for(int i=0;i<homework_list.size();i++)
    {
        CheckItem *item = homework_list[i];
        ui->task_layout->addWidget(item);
        connect(item, &CheckItem::check_click, this, &Homework::removeTask);
        connect(item, &CheckItem::edit_click, this, &Homework::update_setting);
    }
}

Homework::~Homework()
{
    delete ui;
}

// only update local todos
void Homework::update_setting()
{
    QSettings setting("setting.ini",QSettings::IniFormat);
    int len = homework_list.size();
    setting.beginWriteArray("todos");
    for(int i=0;i<len;i++)
    {
        int cnt=0;
        if(homework_list[i]->getIsRemote() == false)
        {
            QString name = homework_list.at(i)->getName();
            QDate ddl = homework_list.at(i)->getDdl();
            setting.setArrayIndex(cnt);
            setting.setValue("name", name);
            setting.setValue("ddl", ddl.toString("dd.MM.yyyy"));
            cnt++;
        }
    }
    setting.endArray();
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

        // add it into setting.ini
        update_setting();
    }
}

void Homework::removeTask(CheckItem *item)
{
    homework_list.removeOne(item);
    ui->task_layout->removeWidget(item);
    item->setParent(0);
    delete item;

    update_setting();
}

void Homework::on_update_clicked()
{
	QJsonArray json = QJsonArray(remoteAPI.fetchRemoteToDos());
    qDebug() << json.size();
    for (int i=0;i<json.size();i++) {
        qDebug() << json.at(i)["deadline"];
        qDebug() << json.at(i)["title"].toString();
    }

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
