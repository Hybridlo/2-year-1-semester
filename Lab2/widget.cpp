#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QDirIterator>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QInputDialog>

void initDrives(Ui::Widget *ui)
{
    QDirIterator it(QDir::currentPath() + "/dirdata", QStringList() << "*.txt", QDir::Files);
    while (it.hasNext())
    {
        it.next();
        QString txtfile = it.fileName();
        txtfile.chop(4);
        ui->listWidget->addItem(txtfile);
    }
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initDrives(ui);
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
        if (drives.at(i).filePath() == "D:/" || drives.at(i).filePath() == "C:/") //skip C: and D: drive
            continue;
        QString text = QInputDialog::getText(0, "Drive",
                                                 "Enter " + drives.at(i).filePath() + " name:", QLineEdit::Normal,
                                                 ""); //assign name for a drive
        text = "dirdata\\" + text + ".txt";  //to create a txt file with drive content

        if (!QDir(QDir::currentPath()).exists("dirdata")) //create a dirdata directory if doesn't exist
            QDir(QDir::currentPath()).mkdir("dirdata");

        QFile fout(text);
        fout.open(QIODevice::WriteOnly);
        QTextStream out(&fout);

        qDebug() << text;
        QDirIterator it(drives.at(i).filePath(), QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (it.hasNext()) {  //scan drive
            it.next();
            QString dir = it.filePath();
            QString newdir = "";
            for (int j = 0; j < dir.size(); j++)
            {
                if (dir[j] == '/')
                    newdir += ' ';
            }
            newdir += it.fileName();
            out << newdir.toUtf8().constData() << '\n';
        }
        fout.close();
    }
}

int dirDepth = 0;
QString currfile;

void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
   if (item->text() == "..")  //parent directory
   {
        QString temp = ui->label->text();
        temp.chop(1);
        ui->label->setText(temp);
        if (temp.lastIndexOf("/") != -1)
          ui->label->setText(temp.mid(0, temp.lastIndexOf("/") + 1)); //remove last directory from working directory path
        else
        ui->label->setText("");
        dirDepth--;
        if (dirDepth == 0)
        {
           ui->listWidget->clear();  //show offline drives if got out of some drive
           initDrives(ui);
           ui->pushButton->setEnabled(true);
           return;
        }
    }
    else
    if (item->text().indexOf(".") != -1)
    {
        return;  //entries with suffix are not a directory
    }
    if (dirDepth == 0)
    {
         currfile = "dirdata/" + item->text() + ".txt"; //drive opened
         ui->pushButton->setEnabled(false);
    }

    if (item->text() != "..")
    {
       ui->label->setText(ui->label->text() + item->text() + '/');
       dirDepth++;
    }

    QString prevpath = ui->label->text(); //working directory
    prevpath.remove(0, prevpath.indexOf("/") + 1);  //remove drive's name

    ui->listWidget->clear();
    ui->listWidget->addItem("..");

    QFile fin(currfile);
    fin.open(QIODevice::ReadOnly);
    QTextStream in(&fin);

    bool outOfDirectory = false;
    bool inPrevpath = false;
    if (prevpath.size() == 0)
        inPrevpath = true;
    while (!in.atEnd() && !outOfDirectory)
       {
          QString line = in.readLine();
          int i = 0, count = 0;

          while (!inPrevpath) //getting back into working directory
          {
              QString nextdir = prevpath.mid(0, prevpath.indexOf("/")); //take 1 directory step
              i++;
              while (line != (QString(' ').repeated(i) + nextdir)) //looking for that directory
                  line = in.readLine();
              prevpath.remove(0, prevpath.indexOf("/") + 1);
              if (prevpath.size() == 0)
                  inPrevpath = true;
              line = in.readLine();
          }

          i = 0;
          bool found = false;
          while (!found)
          {
              if (line[i] == ' ')
                  count++;
              else
              {
                  outOfDirectory = true;  //found all files in directory
                  break;
              }
              i++;
              if (count == dirDepth)
              {
                  if (line[i] != ' ')
                      found = true;  //found file/directory in current directory
                  else
                      break;
              }
          }
          if (found)
          {
              line.remove(0, dirDepth);
              ui->listWidget->addItem(line);
          }
       }
    fin.close();
}
