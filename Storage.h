#ifndef STORAGE_H
#define STORAGE_H

#include <QSettings>
#include <QVector>

class CheckItem;

class Storage : public QVector<CheckItem*>
{
private:
	QSettings storage;
	void backup();
public:
	Storage(const QString = "storage");
	~Storage();
	void refresh(CheckItem*);
};

#endif // STORAGE_H
