#include "airplanner.h"
#include <QApplication>
#include "choice.h"
#include "homework.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Airplanner userDefined;
    Homework homework;

    Choice dlg;
    if (dlg.exec() == QDialog::Accepted)
    {
       userDefined.show();
       return a.exec();
    }
    else if(dlg.exec() == QDialog::Rejected)
    {
        homework.show();
        return a.exec();
    }
    else return 0;

}
