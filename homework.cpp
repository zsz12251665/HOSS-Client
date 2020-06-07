#include "homework.h"
#include "ui_homework.h"
#include "RemoteAPI.h"

RemoteAPI remoteAPI(QString("http://47.112.198.206:8000/"));

Homework::Homework(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Homework),
	homework_list()
{
	ui->setupUi(this);
	ui->dateEdit->setDate(QDateTime::currentDateTime().date());
	QSettings setting("setting.ini",QSettings::IniFormat);

	// get the remote_done todos
	int done_len = setting.beginReadArray("done");
	for(int i=0;i<done_len;i++)
	{
		setting.setArrayIndex(i);
		QString name = setting.value("name").toString();
		QDate ddl = QDate::fromString(setting.value("ddl").toString(), "dd.MM.yyyy");
		QString directory = setting.value("directory").toString();
		CheckItem *item = new CheckItem(name, ddl, true, directory);
		this->homework_done.append(item);
	}
	setting.endArray();

	// get the remote todos
	QJsonArray json = QJsonArray(remoteAPI.fetchRemoteToDos());
	for (int i=0;i<json.size();i++) {
		qDebug() << json.at(i)["deadline"].toString();
		QString name = json.at(i)["title"].toString();
		QDate ddl = QDate::fromString(json.at(i)["deadline"].toString(), "yyyy-MM-dd");
		QString directory = json.at(i)["directory"].toString();
		if(ddl >= QDate::currentDate() && !isDone(name))
		{
			CheckItem *item = new CheckItem(name, ddl, true, directory);
			this->homework_list.append(item);
		}
	}

	// get local todos
	int len = setting.beginReadArray("todos");
	for(int i=0;i<len;i++)
	{
		setting.setArrayIndex(i);
		QString name = setting.value("name").toString();
		QDate ddl = QDate::fromString(setting.value("ddl").toString(), "dd.MM.yyyy");
		CheckItem *item = new CheckItem(name, ddl, false);
		this->homework_list.append(item);
		connect(item, &CheckItem::editEvent, this, &Homework::update_setting);
	}
	setting.endArray();

	// add widgets
	for(int i=0;i<homework_list.size();i++)
	{
		CheckItem *item = homework_list[i];
		ui->task_layout->addWidget(item);
		connect(item, &CheckItem::removeEvent, this, &Homework::removeTask);
		connect(item, &CheckItem::editEvent, this, &Homework::update_setting);
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
    int cnt=0;
    for(int i=0;i<len;i++)
    {
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
		CheckItem *item = new CheckItem(name, ddl, false);
		this->homework_list.append(item);
		ui->task_layout->addWidget(item);
		connect(item, &CheckItem::removeEvent, this, &Homework::removeTask);
		// qDebug() << name << ddl <<  this->homework_list.isEmpty();
		ui->input_task->clear();

		// add it into setting.ini
		update_setting();
	}
}

void Homework::removeTask(CheckItem *item)
{
	if(item->getIsRemote() == false)
	{
		homework_list.removeOne(item);
		ui->task_layout->removeWidget(item);
		item->setParent(0);
		delete item;

		update_setting();
	}
	else {
		// upload the file to the server
		QString directory = item->getDirectory();

		if(upload_file(directory)){
			// add it to the homework_done
			homework_done.append(item);
			QSettings setting("setting.ini",QSettings::IniFormat);
			setting.beginWriteArray("done");
			for(int i=0;i<homework_done.size();i++)
			{
				QString name = homework_done.at(i)->getName();
				QDate ddl = homework_done.at(i)->getDdl();
				QString directory = homework_done.at(i)->getDirectory();
				setting.setArrayIndex(i);
				setting.setValue("name", name);
				setting.setValue("ddl", ddl.toString("dd.MM.yyyy"));
				setting.setValue("directory", directory);
			}
			setting.endArray();

			// delete the item
			homework_list.removeOne(item);
			ui->task_layout->removeWidget(item);
			item->setParent(0);
			delete item;
		}
	}
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
	qDebug() << upload_file("test");
}
bool Homework::upload_file(QString directory)
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open the file");

	if (!fileName.isNull())
	{
		QSettings setting("setting.ini", QSettings::IniFormat);
		QString StuName = setting.value("config/StuName").toString();
		QString StuNumber = setting.value("config/StuNumber").toString();
		QFile file(fileName);
		int reply = remoteAPI.uploadHomework(StuName, StuNumber, directory, file).first;
		return reply == 200;
	}
	return false;
}

void Homework::on_Setting_clicked()
{
	// declare the setting
	QSettings setting("setting.ini",QSettings::IniFormat);

	// generate a multiply input dialog
	QDialog dialog(this);
	QFormLayout form(&dialog);
	form.addRow(new QLabel("Setting: "));

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

bool Homework::isDone(QString name)
{
	int len = homework_done.size();
	for(int i=0;i<len;i++)
	{
		if(name == homework_done.at(i)->getName())
			return true;
	}
	return false;
}

void Homework::clear_all_checkitems()
{
    QLayoutItem *child;
     while ((child = ui->task_layout->takeAt(0)) != 0)
     {
            if(child->widget())
            {
                child->widget()->setParent(NULL);
            }

            delete child;
     }
}

void Homework::on_all_clicked()
{
    // clear all the checkitems
    clear_all_checkitems();

    // add the required checkitems
    for(int i=0;i<homework_list.size();i++)
    {
        CheckItem *item = homework_list[i];
        ui->task_layout->addWidget(item);
        connect(item, &CheckItem::removeEvent, this, &Homework::removeTask);
        connect(item, &CheckItem::editEvent, this, &Homework::update_setting);
    }
}

void Homework::on_homework_clicked()
{
    // clear all the checkitems
    clear_all_checkitems();

    // add the required checkitems
    for(int i=0;i<homework_list.size();i++)
    {
        CheckItem *item = homework_list[i];
        if(item->getIsRemote() == true) {
            ui->task_layout->addWidget(item);
            connect(item, &CheckItem::removeEvent, this, &Homework::removeTask);
            connect(item, &CheckItem::editEvent, this, &Homework::update_setting);
        }
    }
}

void Homework::on_mine_clicked()
{
    // clear all the checkitems
    clear_all_checkitems();

    // add the required checkitems
    for(int i=0;i<homework_list.size();i++)
    {
        CheckItem *item = homework_list[i];
        if(item->getIsRemote() == false) {
            ui->task_layout->addWidget(item);
            connect(item, &CheckItem::removeEvent, this, &Homework::removeTask);
            connect(item, &CheckItem::editEvent, this, &Homework::update_setting);
        }
    }
}

void Homework::on_HasSubmitted_clicked()
{
    // clear all the checkitems
    clear_all_checkitems();

    // add the required checkitems
    for(int i=0;i<homework_done.size();i++)
    {
        CheckItem *item = homework_done[i];
        ui->task_layout->addWidget(item);
        connect(item, &CheckItem::removeEvent, this, &Homework::removeTask);
        connect(item, &CheckItem::editEvent, this, &Homework::update_setting);
    }
}
