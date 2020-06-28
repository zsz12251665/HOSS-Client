#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QMainWindow>

#include "Storage.h"

namespace Ui
{
	class Homework;
}

class Homework : public QMainWindow
{
	Q_OBJECT
protected:
	Ui::Homework *ui;
private:
	enum ShowState
	{
		NONE = 0,	// 00
		LOCAL = 1,	// 01
		REMOTE = 2,	// 10
		ALL = 3		// 11
	};
	Storage list;
	ShowState currentState;
	void addItem(CheckItem*);
	void showItems(ShowState);
private slots:
	void on_button_add_clicked();
	void on_button_new_clicked();
	void on_button_settings_clicked();
	void on_button_update_clicked();
	void on_edit_title_returnPressed();
	void on_radio_all_clicked();
	void on_radio_local_clicked();
	void on_radio_remote_clicked();
	void on_button_help_clicked();
public:
	Homework(QWidget* = nullptr);
	~Homework();
};

#endif // HOMEWORK_H
