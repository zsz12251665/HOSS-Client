#include "Homework.h"
#include "ui_homework.h"

#include "CheckItem.h"
#include "CheckItem_EditDialog.h"
#include "RemoteAPI.h"
#include "Settings.h"

#include <QDebug>

static inline ShowState getState(CheckItem *item)
{
	return item->isDeleted() || (item->isRemote() && item->getDeadline() <
								 QDate::currentDate()) ? ShowState::NONE
													   : item->isRemote() ? ShowState::REMOTE
																		  : ShowState::LOCAL;
}

Homework::Homework(QWidget *parent) : QMainWindow(parent), ui(new Ui::Homework),
	storage(&list), currentState(ShowState::ALL)
{
	// Initialize the server URL
	Settings settings;
	if (settings.getServer().isEmpty())
		settings.popEditDialog();
	// Initialize UI
	ui->setupUi(this);
	ui->edit_deadline->setDate(QDate::currentDate());
	for (int i = 0; i < list.size(); ++i)
	{
		connect(list.at(i), &CheckItem::editEvent, this, &Homework::refresh_storage);
		ui->layout_todos->addWidget(list.at(i));
	}
	on_button_update_clicked();
	on_radio_all_clicked();
}

Homework::~Homework()
{
	delete ui;
}

void Homework::addItem(CheckItem *item)
{
	list.push_back(item);
	storage.refresh(item);
	connect(item, &CheckItem::editEvent, this, &Homework::refresh_storage);
	ui->layout_todos->addWidget(item);
	item->setVisible(currentState & getState(item));
}

void Homework::showItems(ShowState newState)
{
	currentState = newState;
	for (int i = 0; i < list.size(); ++i)
	{
		list.at(i)->setVisible(currentState & getState(list.at(i)));
	}
}

void Homework::on_button_add_clicked()
{
	on_edit_title_returnPressed();
}

void Homework::on_button_new_clicked()
{
	CheckItem_EditDialog editDialog;
	if (editDialog.exec() == QDialog::Accepted && !editDialog.titleValue().isEmpty())
		addItem(new CheckItem(list.size(), editDialog.titleValue(),
							  editDialog.deadlineValue()));
}

void Homework::on_button_settings_clicked()
{
	Settings().popEditDialog();
	on_button_update_clicked();
}

void Homework::on_button_update_clicked()
{
	qDebug() << "Homework::on_button_update_clicked() Starts";
	QJsonArray remoteList = RemoteAPI(Settings().getServer()).fetchRemoteToDos();
	// Remove the out-of-date ones
	for (int i = 0; i < remoteList.size(); ++i)
		if (remoteList.at(i)["deadline"].toVariant().toDate() < QDate::currentDate())
		{
			remoteList.removeAt(i--);
		}
	for (int i = 0; i < list.size(); ++i)
		if (!list.at(i)->isDeleted())
		{
			bool isOutOfDate = list.at(i)->isRemote();
			// Remove the existing ones
			for (int j = 0; j < remoteList.size(); ++j)
			{
				QString title = remoteList.at(j)["title"].toString();
				QDate deadline = remoteList.at(j)["deadline"].toVariant().toDate();
				QString directory = remoteList.at(j)["directory"].toString();
				if (list.at(i)->getTitle() == title && list.at(i)->getDirectory() == directory
					&& list.at(i)->getDeadline() == deadline)
				{
					isOutOfDate = false;
					remoteList.removeAt(j--);
				}
			}
			// Remove the out-of-date ones in the local list
			if (isOutOfDate)
				list.at(i)->deleteItem();
		}
	for (int i = 0; i < remoteList.size(); ++i)
	{
		QString title = remoteList.at(i)["title"].toString();
		QDate deadline = remoteList.at(i)["deadline"].toVariant().toDate();
		QString directory = remoteList.at(i)["directory"].toString();
		qDebug() << title << deadline << directory;
		addItem(new CheckItem(list.size(), title, deadline, directory));
	}
	qDebug() << "Homework::on_button_update_clicked() Ends" << endl;
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

void Homework::on_edit_title_returnPressed()
{
	if (!ui->edit_title->text().isEmpty())
	{
		QString title = ui->edit_title->text();
		QDate deadline = ui->edit_deadline->date();
		addItem(new CheckItem(list.size(), title, deadline));
		ui->edit_title->setText(QString());
		ui->edit_deadline->setDate(QDate::currentDate());
	}
}

void Homework::refresh_storage(CheckItem *item)
{
	storage.refresh(item);
}
