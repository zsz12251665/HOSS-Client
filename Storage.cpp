#include "Storage.h"

#include "CheckItem.h"

#include <QDebug>

Storage::Storage(QVector<CheckItem*> *list, const QString filename) :
	storage(filename + ".ini", QSettings::IniFormat), list(list)
{
	qDebug() << "Storage::Storage() Starts";
	if (filename.isEmpty())
	{
		return;
	}
	list->clear();
	// Import the todo list from local storage
	int length = storage.beginReadArray("todos");
	qDebug() << length;
	for (int i = 0; i < length; ++i)
	{
		storage.setArrayIndex(i);
		QString title = storage.value("title").toString();
		QDate deadline = storage.value("deadline").toDate();
		QString directory = storage.value("directory").toString();
		bool isFinished = storage.value("isFinished").toBool();
		qDebug() << title << deadline << directory << isFinished;
		if (!title.isEmpty() && !deadline.isNull() && (directory.isEmpty() ||
													  deadline >= QDate::currentDate()))
		{
			list->push_back(new CheckItem(list->size(), title, deadline, directory,
										 isFinished));
		}
	}
	storage.endArray();
	qDebug() << "Storage::Storage() Ends" << endl;
}

Storage::~Storage()
{
	qDebug() << "Storage::~Storage() Starts";
	// Back up the todo list from local storage
	storage.remove("todos");
	storage.beginWriteArray("todos");
	for (int i = 0, cnt = 0; i < list->size(); ++i)
		if (!list->at(i)->isDeleted())
		{
			qDebug() << cnt << list->at(i)->getTitle() << list->at(i)->getDeadline()
					 << list->at(i)->getDirectory() << list->at(i)->getIsFinished();
			storage.setArrayIndex(cnt++);
			storage.setValue("title", list->at(i)->getTitle());
			storage.setValue("deadline", list->at(i)->getDeadline());
			storage.setValue("directory", list->at(i)->getDirectory());
			storage.setValue("isFinished", list->at(i)->getIsFinished());
		}
	storage.endArray();
	storage.sync();
	qDebug() << "Storage::~Storage() Ends" << endl;
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
		storage.setValue("isFinished", item->getIsFinished());
	}
	storage.endArray();
	storage.sync();
}

