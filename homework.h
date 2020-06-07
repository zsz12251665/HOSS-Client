#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QDate>
#include <QDateEdit>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QJsonArray>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QSettings>
#include <QVector>
#include "checkitem.h"

namespace Ui
{
	class Homework;
}

class Homework : public QMainWindow
{
	Q_OBJECT
private:
	Ui::Homework *ui;
	QVector<CheckItem*> homework_list;
	QVector<CheckItem*> homework_done;
private slots:
	void on_input_task_returnPressed();
	void on_update_clicked();
	void on_upload_clicked();
	void on_Setting_clicked();
public:
	Homework(QWidget *parent = nullptr);
	~Homework();
public slots:
	void removeTask(CheckItem *item);
	void update_setting();
	bool upload_file(QString directory);
	bool isDone(QString name);
};

#endif // HOMEWORK_H
