#include "airplanner.h"
#include <QApplication>
#include "choice.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Airplanner w;

   Choice dlg;
       if (dlg.exec() == QDialog::Accepted)
       {
          w.show();
          return a.exec();
       }
       else return 0;
}
