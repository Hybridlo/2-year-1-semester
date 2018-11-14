#include "fileinfowindow.h"
#include "ui_fileinfowindow.h"

FileInfoWindow::FileInfoWindow(QWidget *parent, QMap<QString, QString> meta) :
    QDialog(parent),
    ui(new Ui::FileInfoWindow)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //make columns stretch
    ui->tableWidget->insertColumn(0);

    QTableWidgetItem *item = new QTableWidgetItem("Value");
    item->setFlags(Qt::NoItemFlags); //makes new item unselectable

    ui->tableWidget->setHorizontalHeaderItem(0, item); //add column with name "value"

    int j = 0;
    QMapIterator<QString, QString> i(meta);
    while (i.hasNext())
    {
        i.next();
        ui->tableWidget->insertRow(j); //add row

        item = new QTableWidgetItem(i.key());
        item->setFlags(Qt::NoItemFlags);
        ui->tableWidget->setVerticalHeaderItem(j, item); //name of the row is the key
        int size = i.value().toInt();
        int level = 0;
        if (item->text() == "Size")
        {
            QString stringLevel;
            while (size > 1024)
            {
                size = size / 1024;
                level++;
            }
            if (level == 0)
                stringLevel = " byte";
            if (level == 1)
                stringLevel = " KB";
            if (level == 2)
                stringLevel = " MB";
            if (level == 3)
                stringLevel = " GB";
            item = new QTableWidgetItem(QString::number(size) + stringLevel);
        }
        else
        item = new QTableWidgetItem(i.value()); //item with value
        item->setFlags(Qt::NoItemFlags);
        ui->tableWidget->setItem(j,0, item); //set that item in table

        j++;
    }
    metadata = meta; //transfer outside of the constructor to use in slot
}

FileInfoWindow::~FileInfoWindow()
{
    delete ui;
}

void FileInfoWindow::on_pushButton_clicked()
{
    close();
}

void FileInfoWindow::on_pushButton_2_clicked()
{
    SearchByMap search(this, metadata);
    search.exec();
}
