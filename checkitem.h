#ifndef CHECKITEM_H
#define CHECKITEM_H

#include <QWidget>
#include <QDate>
#include <QDebug>
#include <QInputDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>

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
private slots:
    void on_Edit_button_clicked();
};

#endif // CHECKITEM_H
