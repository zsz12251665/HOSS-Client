#ifndef AIRPLANNER_H
#define AIRPLANNER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Airplanner; }
QT_END_NAMESPACE

class Airplanner : public QMainWindow
{
    Q_OBJECT

public:
    Airplanner(QWidget *parent = nullptr);
    ~Airplanner();

public slots:
    void submitButton();
    void removeButton();
    void exitButton();

private:
    Ui::Airplanner *ui;
};
#endif // AIRPLANNER_H
