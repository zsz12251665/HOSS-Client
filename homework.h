#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QMainWindow>
#include <QVector>
#include "checkitem.h"
#include "ui_checkitem.h"
#include "remoteAPI.h"
#include <QJsonArray>
#include <QFileDialog>
#include <QSettings>

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

private slots:

    void on_input_task_returnPressed();

    void on_update_clicked();

    void on_upload_clicked();

    void on_Setting_clicked();

private:
    Ui::Homework *ui;
    QVector<CheckItem*> homework_list;
};

#endif // HOMEWORK_H
