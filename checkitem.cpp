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

void CheckItem::on_Edit_button_clicked()
{
    // generate a multiply input dialog
    QDialog dialog(this);
    QFormLayout form(&dialog);
    form.addRow(new QLabel("Edit the task:"));
    QString value1 = QString("name: ");
    QSpinBox *spinbox1 = new QSpinBox(&dialog);
    form.addRow(value1, spinbox1);
    QString value2 = QString("date: ");
    QSpinBox *spinbox2 = new QSpinBox(&dialog);
    form.addRow(value2, spinbox2);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process if OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here

    }

}
