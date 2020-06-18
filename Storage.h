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
	Storage(QString = "storage");
	~Storage();
	void push_back(CheckItem*);
	void refresh(const CheckItem*);
};

#endif // STORAGE_H
