#ifndef CHECKITEM_H
#define CHECKITEM_H

#include <QWidget>
#include <QDate>
#include <QDebug>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QDateEdit>

namespace Ui {
class CheckItem;
}

class CheckItem : public QWidget
{
    Q_OBJECT

public:
    explicit CheckItem(QString name, QDate ddl, bool isRemote=false, QString directory="" ,QWidget *parent = nullptr);
    ~CheckItem();

    QString getName();
    QDate getDdl();
    bool getIsRemote();
    QString getDirectory();

private:
    Ui::CheckItem *ui;
    QString name;
    QDate ddl;
    bool isRemote = false;
    QString directory = "";

signals:
    void check_click(CheckItem *item);
    void edit_click();
private slots:
    void on_Edit_button_clicked();
};

#endif // CHECKITEM_H
