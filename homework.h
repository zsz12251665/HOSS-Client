#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QMainWindow>
#include <QVector>
#include <QJsonArray>
#include <QFileDialog>
#include <QSettings>
#include <QDate>
#include <QDebug>
#include <QLabel>
#include "checkitem.h"

namespace Ui {
class Homework;
}

class Homework : public QMainWindow
{
	Q_OBJECT

public:
	explicit Homework(QWidget *parent = nullptr);
	~Homework();

public slots:
	void removeTask(CheckItem *item);
	void update_setting();
    void upload_file(QString directory);
    bool isDone(QString name);

private slots:

	void on_input_task_returnPressed();

	void on_update_clicked();

	void on_upload_clicked();

	void on_Setting_clicked();

private:
	Ui::Homework *ui;
	QVector<CheckItem*> homework_list;
    QVector<CheckItem*> homework_done;
};

#endif // HOMEWORK_H
