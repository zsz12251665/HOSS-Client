#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QMainWindow>

#include "Storage.h"

namespace Ui
{
	class Homework;
}

enum ShowState
{
	NONE = 0, // 00
	LOCAL = 1, // 01
	REMOTE = 2, // 10
	ALL = 3 // 11
};

class Homework : public QMainWindow
{
	Q_OBJECT
private:
	Ui::Homework *ui;
	QVector<CheckItem*> list;
	Storage storage;
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
	void refresh_storage(CheckItem*);
public:
	Homework(QWidget *parent = nullptr);
	~Homework();
};

#endif // HOMEWORK_H
