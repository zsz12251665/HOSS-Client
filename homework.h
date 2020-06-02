#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QMainWindow>
<<<<<<< HEAD
#include <QVector>
#include "checkitem.h"
#include "ui_checkitem.h"
=======
>>>>>>> 1dfc476f1df8bd2e60ccdfaea90b86a4e6d1c4a7

namespace Ui {
class Homework;
}

class Homework : public QMainWindow
{
    Q_OBJECT

public:
    explicit Homework(QWidget *parent = nullptr);
    ~Homework();

<<<<<<< HEAD
public slots:
    void removeTask(CheckItem *item);

=======
>>>>>>> 1dfc476f1df8bd2e60ccdfaea90b86a4e6d1c4a7
private slots:

    void on_Add_button_clicked();

<<<<<<< HEAD
=======

>>>>>>> 1dfc476f1df8bd2e60ccdfaea90b86a4e6d1c4a7
    void on_input_task_returnPressed();

private:
    Ui::Homework *ui;
<<<<<<< HEAD
    QVector<CheckItem*> homework_list;
=======
>>>>>>> 1dfc476f1df8bd2e60ccdfaea90b86a4e6d1c4a7
};

#endif // HOMEWORK_H
