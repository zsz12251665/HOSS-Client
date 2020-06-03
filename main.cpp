#include <QApplication>
#include "choice.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    Choice dlg;
	dlg.show();
	return a.exec();
}
