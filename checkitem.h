#ifndef CHECKITEM_H
#define CHECKITEM_H

#include <QWidget>
#include <QDate>
#include <QDebug>

namespace Ui {
class CheckItem;
}

class CheckItem : public QWidget
{
    Q_OBJECT

public:
    explicit CheckItem(QString name, QDate ddl, QWidget *parent = nullptr);
    ~CheckItem();

private:
    Ui::CheckItem *ui;
    QString name;
    QDate ddl;

signals:
    void check_click(CheckItem *item);
    void edit_click(CheckItem *item);
public slots:
    //void edit_item(CheckItem *item);
};

#endif // CHECKITEM_H
