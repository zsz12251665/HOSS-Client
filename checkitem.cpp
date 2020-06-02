#include "checkitem.h"
#include "ui_checkitem.h"

CheckItem::CheckItem(QString name, QDate ddl, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckItem)
{
    ui->setupUi(this);

    this->name = name;
    this->ddl = ddl;

    ui->checkBox->setText(name);
    ui->label->setText(ddl.toString("dd.MM.yyyy"));

    connect(ui->checkBox,&QCheckBox::toggled,
            [this] {
                    emit check_click(this);
                   });
}

CheckItem::~CheckItem()
{
    delete ui;
}
