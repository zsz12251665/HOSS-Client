#ifndef STORAGE_H
#define STORAGE_H

#include <QSettings>
#include <QVector>

class CheckItem;

class Storage
{
private:
	QSettings storage;
	QVector<CheckItem*>* list;
public:
	Storage(QVector<CheckItem*>*, const QString = "storage");
	~Storage();
	void refresh(CheckItem*);
};

#endif // STORAGE_H
