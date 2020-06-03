#ifndef CHOICE_H
#define CHOICE_H

#include <QMainWindow>

namespace Ui {
class Choice;
}

class Choice : public QMainWindow
{
    Q_OBJECT

public:
	explicit Choice(QWidget *parent = nullptr);
    ~Choice();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Choice *ui;
};

#endif // CHOICE_H
