#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <QMainWindow>
#include <QVector>

class CheckItem;

namespace Ui
{
	class Homework;
}


class Homework : public QMainWindow
{
	Q_OBJECT
private:
	Ui::Homework *ui;
	QVector<CheckItem*> homework_list;
	QVector<CheckItem*> homework_done;
private slots:
	void on_button_settings_clicked();
	void on_button_update_clicked();
	void on_radio_all_clicked();
	void on_radio_homework_clicked();
	void on_radio_mine_clicked();
	void on_input_task_returnPressed();
	void on_HasSubmitted_clicked();
public:
	Homework(QWidget *parent = nullptr);
	~Homework();
public slots:
	void removeTask(CheckItem *item);
	void update_setting();
	bool upload_file(QString directory);
	bool isDone(QString name);
	void clear_all_checkitems();
	void initial_todos();
};

#endif // HOMEWORK_H
