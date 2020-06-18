#include "Storage.h"

#include "CheckItem.h"

#include <QDebug>

Storage::Storage(const QString filename)
	: storage(filename + ".ini", QSettings::IniFormat)
{
	qDebug() << "Storage::Storage() Starts";
	if (filename.isEmpty())
		return;
	clear();
	// Import the todo list from local storage
	for (int i = 0, length = storage.beginReadArray("todos"); i < length; ++i)
	{
		storage.setArrayIndex(i);
		QString title = storage.value("title").toString();
		QDate deadline = storage.value("deadline").toDate();
		QString directory = storage.value("directory").toString();
		bool isFinished = storage.value("checked").toBool();
		qDebug() << title << deadline << directory << isFinished;
		// Only local ones or unexpired ones can be added
		if (directory.isEmpty())
			push_back(new LocalCheckItem(size(), title, deadline, isFinished));
		else
			if (deadline >= QDate::currentDate())
				push_back(new RemoteCheckItem(size(), title, deadline, directory, isFinished));
	}
	storage.endArray();
	// Sync local storage with vector list
	backup();
	qDebug() << "Storage::Storage() Ends" << endl;
}

Storage::~Storage()
{
	// Backup the list
	backup();
	// Free memory space
	while (!empty())
	{
		delete back();
		pop_back();
	}
}

void Storage::backup()
{
	// Export the todo list to local storage
	qDebug() << "Storage::backup() Starts";
	storage.remove("todos");
	storage.beginWriteArray("todos");
	for (int i = 0, cnt = 0; i < size(); ++i)
		if (!at(i)->isDeleted())
		{
			QString title = at(i)->getTitle();
			QDate deadline = at(i)->getDeadline();
			bool checked = at(i)->isFinished();
			qDebug() << cnt << title << deadline << checked;
			storage.setArrayIndex(cnt++);
			storage.setValue("title", title);
			storage.setValue("deadline", deadline);
			storage.setValue("checked", checked);
			if (at(i)->isRemote())
				storage.setValue("directory", static_cast<RemoteCheckItem*>(at(i))->getDirectory());
		}
	storage.endArray();
	storage.sync();
	qDebug() << "Storage::backup() Ends" << endl;
}

void Storage::push_back(CheckItem *item)
{
	QVector::push_back(item);
	QObject::connect(item, &CheckItem::editEvent, [this](CheckItem *item){this->refresh(item);});
}

void Storage::refresh(const CheckItem *item)
{
	// Sync single item with local storage
	qDebug() << "Storage::refresh() Starts";
	storage.beginWriteArray("todos");
	storage.setArrayIndex(item->getId());
	if (item->isDeleted())
		storage.remove("");
	else
	{
		QString title = item->getTitle();
		QDate deadline = item->getDeadline();
		bool checked = item->isFinished();
		qDebug() << item->getId() << title << deadline << checked;
		storage.setValue("title", title);
		storage.setValue("deadline", deadline);
		storage.setValue("checked", checked);
		if (item->isRemote())
			storage.setValue("directory", static_cast<const RemoteCheckItem*>(item)->getDirectory());
	}
	storage.endArray();
	storage.sync();
	qDebug() << "Storage::refresh() Ends";
}
