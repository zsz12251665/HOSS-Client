#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QMainWindow>

namespace Ui {
class Homework;
}

class Homework : public QMainWindow
{
    Q_OBJECT

public:
    explicit Homework(QWidget *parent = nullptr);
    ~Homework();

private slots:

    void on_Add_button_clicked();


    void on_input_task_returnPressed();

private:
    Ui::Homework *ui;
};

#endif // HOMEWORK_H
