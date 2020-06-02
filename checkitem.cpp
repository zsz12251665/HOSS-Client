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
    QLineEdit *name_input = new QLineEdit(&dialog);
    name_input->setText(this->name);
    form.addRow(value1, name_input);
    QString value2 = QString("date: ");
    QDateEdit *date_input = new QDateEdit(&dialog);
    date_input->setDate(this->ddl);
    form.addRow(value2, date_input);
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    // Process if OK button is clicked
    if (dialog.exec() == QDialog::Accepted) {
        // Do something here
        qDebug() << date_input->date() << name_input->text();
        this->name = name_input->text();
        this->ddl = date_input->date();
        ui->checkBox->setText(name);
        ui->label->setText(ddl.toString("dd.MM.yyyy"));
    }

}
