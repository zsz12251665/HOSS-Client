#include "Storage.h"

#include "CheckItem.h"

#include <QDebug>

Storage::Storage(const QString filename) : storage(filename + ".ini", QSettings::IniFormat)
{
	qDebug() << "Storage::Storage() Starts";
	if (filename.isEmpty())
	{
		return;
	}
	clear();
	// Import the todo list from local storage
	int length = storage.beginReadArray("todos");
	qDebug() << length;
	for (int i = 0; i < length; ++i)
	{
		storage.setArrayIndex(i);
		QString title = storage.value("title").toString();
		QDate deadline = storage.value("deadline").toDate();
		QString directory = storage.value("directory").toString();
		bool isFinished = storage.value("checked").toBool();
		qDebug() << title << deadline << directory << isFinished;
		if (!title.isEmpty() && !deadline.isNull() && (directory.isEmpty() ||
													  deadline >= QDate::currentDate()))
		{
			push_back(new CheckItem(size(), title, deadline, directory, isFinished));
		}
	}
	storage.endArray();
	// Sync the local storage with the vector list
	backup();
	qDebug() << "Storage::Storage() Ends" << endl;
}

Storage::~Storage()
{
	backup();
}

void Storage::backup()
{
	qDebug() << "Storage::backup() Starts";
	// Back up the todo list from local storage
	storage.remove("todos");
	storage.beginWriteArray("todos");
	for (int i = 0, cnt = 0; i < size(); ++i)
		if (!at(i)->isDeleted())
		{
			qDebug() << cnt << at(i)->getTitle() << at(i)->getDeadline()
					 << at(i)->getDirectory() << at(i)->isFinished();
			storage.setArrayIndex(cnt++);
			storage.setValue("title", at(i)->getTitle());
			storage.setValue("deadline", at(i)->getDeadline());
			storage.setValue("directory", at(i)->getDirectory());
			storage.setValue("checked", at(i)->isFinished());
		}
	storage.endArray();
	storage.sync();
	qDebug() << "Storage::backup() Ends" << endl;
}

void Storage::refresh(CheckItem *item)
{
	storage.beginWriteArray("todos");
	storage.setArrayIndex(item->getId());
	if (!item->isDeleted())
		storage.remove("");
	else
	{
		storage.setValue("title", item->getTitle());
		storage.setValue("deadline", item->getDeadline());
		storage.setValue("directory", item->getDirectory());
		storage.setValue("checked", item->isFinished());
	}
	storage.endArray();
	storage.sync();
}
