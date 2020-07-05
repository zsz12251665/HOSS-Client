#include "Homework.h"
#include "Settings.h"

#include <QApplication>
#include <QTranslator>

static QApplication *app = nullptr;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	app = &a;
	// Initialize server URL if none
	Settings settings;
	if (settings.getServer().isEmpty() && !settings.popEditDialog())
		return 0;
	// Set langauge
	changeLanguage(settings.getLanguage());
	Homework homework;
	homework.show();
	return a.exec();
}

void changeLanguage(QString language)
{
	static QTranslator *translator = nullptr;
	if (translator != nullptr)
		delete translator;
	translator = new QTranslator;
	translator->load(QString(":/translations/Translation_%1.qm").arg(language));
	app->installTranslator(translator);
}
