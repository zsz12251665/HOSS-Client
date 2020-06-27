#include "Homework.h"
#include "ui_Homework.h"

#include "CheckItem.h"
#include "CheckItem_EditDialog.h"
#include "NetworkAPI.h"
#include "Settings.h"

#include <QDebug>
#include <QJsonArray>

Homework::Homework(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::Homework), currentState(ShowState::ALL)
{
	// Initialize server URL if none
	Settings settings;
	if (settings.getServer().isEmpty() && !settings.popEditDialog())
		exit(0);
	// Initialize UI
	ui->setupUi(this);
	ui->edit_deadline->setDate(QDate::currentDate());
	// Add items to layout
	for (int i = 0; i < list.size(); ++i)
		ui->layout_todos->addWidget(list.at(i));
	// Sync the remote list
	on_button_update_clicked();
	// Show the items
	showItems(currentState);
}

Homework::~Homework()
{
	delete ui;
}

void Homework::addItem(CheckItem *item)
{
	list.push_back(item);
	list.refresh(item);
	ui->layout_todos->addWidget(item);
	item->setVisible(currentState & item->getShowState());
}

void Homework::showItems(const ShowState newState)
{
	currentState = newState;
	for (int i = 0; i < list.size(); ++i)
		list.at(i)->setVisible(currentState & list.at(i)->getShowState());
}

void Homework::on_button_add_clicked()
{
	// Add a new item according the input
	if (!ui->edit_title->text().isEmpty())
	{
		addItem(new LocalItem(list.size(), ui->edit_title->text(), ui->edit_deadline->date()));
		// Reset the input
		ui->edit_title->setText(QString());
		ui->edit_deadline->setDate(QDate::currentDate());
	}
}

void Homework::on_button_new_clicked()
{
	// Pop a dialog to create a new item
	CheckItem_EditDialog editDialog;
	if (editDialog.exec() == QDialog::Accepted)
		addItem(new LocalItem(list.size(), editDialog.getTitle(), editDialog.getDeadline()));
}

void Homework::on_button_settings_clicked()
{
	// Pop a dialog to edit configurations
	Settings().popEditDialog();
	// Update according to new settings
	on_button_update_clicked();
}

void Homework::on_button_update_clicked()
{
	// Sync remote items with server
	qDebug() << "Homework::on_button_update_clicked() Starts";
	QPair<int, QJsonArray> fetchResult = NetworkAPI::fetchRemoteToDos(Settings());
	// Check if offline
	if (fetchResult.first != 200)
	{
		qDebug() << "Fail to fetch Remote Todos";
		qDebug() << "Homework::on_button_update_clicked() Ends" << endl;
		return;
	}
	// Remove out-of-date ones
	QJsonArray remoteList = fetchResult.second;
	for (int i = 0; i < remoteList.size(); ++i)
		if (remoteList.at(i)["deadline"].toVariant().toDate() < QDate::currentDate())
			remoteList.removeAt(i--);
	for (int i = 0; i < list.size(); ++i)
		if (!list.at(i)->isDeleted() && list.at(i)->isRemote())
		{
			bool isOutOfDate = true;
			// Remove existing ones
			for (int j = 0; j < remoteList.size(); ++j)
				if (static_cast<RemoteItem*>(list.at(i))->isSame(remoteList.at(j)))
				{
					isOutOfDate = false;
					// Update the submission status of remote homework
					list.at(i)->setChecked(remoteList.at(j)["checked"].toBool());
					remoteList.removeAt(j--);
					break;
				}
			// Remove out-of-date ones in the local list
			if (isOutOfDate)
				list.at(i)->remove();
		}
	// Add new ones
	for (int i = 0; i < remoteList.size(); ++i)
	{
		QString title = remoteList.at(i)["title"].toString();
		QDate deadline = remoteList.at(i)["deadline"].toVariant().toDate();
		QString directory = remoteList.at(i)["directory"].toString();
		bool checked = remoteList.at(i)["checked"].toBool();
		qDebug() << title << deadline << directory << checked;
		addItem(new RemoteItem(list.size(), title, deadline, directory, checked));
	}
	qDebug() << "Homework::on_button_update_clicked() Ends" << endl;
    // reset the background
    Settings settings;
    if(!settings.getBackground().isEmpty()) {
        // TODO: save the background picture in the build folder
        // set the background picture
        QPalette PAllbackground = this->palette();
        QImage ImgAllbackground(settings.getBackground());
        QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
        PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
        this->setPalette(PAllbackground);
    }
}

void Homework::on_edit_title_returnPressed()
{
	on_button_add_clicked();
}

void Homework::on_radio_all_clicked()
{
	showItems(ShowState::ALL);
}

void Homework::on_radio_local_clicked()
{
	showItems(ShowState::LOCAL);
}

void Homework::on_radio_remote_clicked()
{
	showItems(ShowState::REMOTE);
}

void Homework::on_button_help_clicked()
{

}

void Homework::resizeEvent(QResizeEvent *)
{
    // reset the background
    Settings settings;
    if(!settings.getBackground().isEmpty()) {
        // TODO: save the background picture in the build folder
        // set the background picture
        QPalette PAllbackground = this->palette();
        QImage ImgAllbackground(settings.getBackground());
        QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
        PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
        this->setPalette(PAllbackground);
    }
}
