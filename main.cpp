#include "Homework.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Homework homework;
	homework.show();
	return a.exec();
}
