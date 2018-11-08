#include "widget.h"
#include "ui_widget.h"
#include <fstream>
#include <QDirIterator>
#include <QStringList>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QInputDialog>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QDirIterator it(QDir::currentPath(), QStringList() << "*.txt", QDir::Files, QDirIterator::Subdirectories);
    /*while (it.hasNext())
    {
        
    }*/
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    QFileInfoList drives;
    drives = QDir::drives();
    for (int i = 1; i < drives.size(); i++)
    {
        if (drives.at(i).filePath() == "D:/")
            continue;
        QString text = QInputDialog::getText(0, "Drive",
                                                 "Enter " + drives.at(i).filePath() + " name:", QLineEdit::Normal,
                                                 "");
        text += ".txt";
        std::ofstream fout(text.toUtf8().constData());
        fout << drives.at(i).filePath().toUtf8().constData() << '\n';
        QDirIterator it(drives.at(i).filePath(), QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            QString dir = it.filePath();
            QString newdir = "";
            qDebug() << dir;
            for (int j = 0; j < dir.size(); j++)
            {
                if (dir[j] == '/')
                    newdir += ' ';
            }
            newdir += it.fileName();
            fout << newdir.toUtf8().constData() << '\n';
        }
        fout.close();
    }
}
