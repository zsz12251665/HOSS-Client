#include <QApplication>
#include "Homework.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Homework homework;
	homework.show();
	return a.exec();
}
