#include "usersearch.h"
#include "ui_usersearch.h"

UserSearch::UserSearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserSearch)
{
    ui->setupUi(this);
}

UserSearch::~UserSearch()
{
    delete ui;
}

void UserSearch::on_pushButton_2_clicked()
{
    close();
}

void UserSearch::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Name")
        ui->label_2->setText("Write file name without suffix");
    if (arg1 == "Created" || arg1 == "Last modified")
        ui->label_2->setText("Format of the date and time: \"dd.mm.yyyy hh:mm:ss\"");
    if (arg1 == "SHA-1 Hash")
        ui->label_2->setText("Enter in hex format");
    if (arg1 == "User notes")
        ui->label_2->setText("");
}

void UserSearch::on_pushButton_clicked()
{
    if (ui->lineEdit->text() == "")
        return;
    QMap<QString, QString> searchBy;
    searchBy[ui->comboBox->currentText()] = ui->lineEdit->text();
    SearchByMap search(this, searchBy, true);
    search.exec();
}
