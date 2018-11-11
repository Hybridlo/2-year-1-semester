#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QDirIterator>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QInputDialog>
#include <QUrl>
#include <QDesktopServices>
#include <QDateTime>
#include <QCryptographicHash>
#include <QTextCodec>

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

int getBackInDir(QTextStream *in, Ui::Widget *ui, QString exactFile)
{
    QString prevpath = ui->label->text() + exactFile; //working directory
    prevpath.remove(0, prevpath.indexOf("/") + 1);  //remove drive's custom name
    bool inPrevpath = false;
    if (prevpath.size() == 0)
        inPrevpath = true;
    QString line;
    int i = 0, linenum = 0;
    while (!inPrevpath)
    {
        line = in->readLine();
        line.remove(line.indexOf("\"") - 1, line.size());
        linenum++;
        QString nextdir = prevpath.mid(0, prevpath.indexOf("/")); //take 1 directory step
        i++;
        while (line != (QString(' ').repeated(i) + nextdir)) //looking for that directory
        {
            line = in->readLine();
            line.remove(line.indexOf("\"") - 1, line.size());
            linenum++;
        }
        if (prevpath.indexOf("/") != -1)
            prevpath.remove(0, prevpath.indexOf("/") + 1);
        else  //no "/" - means file found
            prevpath = "";
        if (prevpath.size() == 0)
            inPrevpath = true;
    }
    return linenum + 1; //+1 for skipped drive name
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initDrives(ui);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
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
        QString text = "";
        while (text == "")
        {
           bool ok;
           text = QInputDialog::getText(0, "Drive",
                                           "Enter " + drives.at(i).filePath() + " name:", QLineEdit::Normal,
                                           "", &ok); //assign name for a drive
           if (!ok)
               return;
        }
        text = "dirdata\\" + text + ".txt";  //to create a txt file with drive content

        if (!QDir(QDir::currentPath()).exists("dirdata")) //create a dirdata directory if doesn't exist
            QDir(QDir::currentPath()).mkdir("dirdata");

        QFile fout(text);
        fout.open(QIODevice::WriteOnly);
        QTextStream out(&fout);
        out << drives.at(i).filePath() << '\n';

        QDirIterator it(drives.at(i).filePath(), QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while (it.hasNext()) {  //scan drive
            it.next();
            QString dir = it.filePath();
            QFileInfo info(dir);
            QString newdir = "";
            for (int j = 0; j < dir.size(); j++)
            {
                if (dir[j] == '/')
                    newdir += ' ';
            }
            newdir += it.fileName();
            QCryptographicHash crypto(QCryptographicHash::Sha1);
            QFile hashing(it.filePath());
            hashing.open(QFile::ReadOnly);
            while(!hashing.atEnd())
              crypto.addData(hashing.read(8192));
            QByteArray hash = crypto.result();
            qDebug() << hash.toHex();
            out << newdir << " \"" << info.created().toString("dd.MM.yyyy HH:mm:ss")
                << "\" \"" << info.lastModified().toString("dd.MM.yyyy HH:mm:ss") << "\"";
            if (it.fileName().indexOf('.') != -1) //directories have same hash
                out << " \"" << hash.toHex() << "\"";
            out << " \"\"" << "\n"; //adding last "" for user notes
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
           ui->pushButton_2->setEnabled(false);
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
         ui->pushButton_2->setEnabled(true);
    }

    if (item->text() != "..")
    {
       ui->label->setText(ui->label->text() + item->text() + '/');
       dirDepth++;
    }

    QString prevpath = ui->label->text(); //working directory
    prevpath.remove(0, prevpath.indexOf("/") + 1);  //remove drive's custom name

    ui->listWidget->clear();
    ui->listWidget->addItem("..");

    QFile fin(currfile);
    fin.open(QIODevice::ReadOnly);
    QTextStream in(&fin);
    in.readLine();  //don't read drive's name

    getBackInDir(&in, ui, "");  //getting back into working directory

    bool outOfDirectory = false;
    while (!in.atEnd() && !outOfDirectory)
       {
          QString line = in.readLine();
          line.remove(line.indexOf("\"") - 1, line.size());
          int i = 0, count = 0;

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

void Widget::on_pushButton_2_clicked()
{
    QFile fin(currfile);
    fin.open(QIODevice::ReadOnly);
    QTextStream in(&fin);
    QString drive = in.readLine();  //get drive name
    fin.close();
    QString path = ui->label->text();  //get path to curent dir
    path.remove(0, path.indexOf("/") + 1);  //remove custom name
    path = drive + path;  //make a full path
    QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
}

void Widget::on_pushButton_3_clicked()
{
    QFile fin(currfile);
    fin.open(QIODevice::ReadOnly);
    QTextStream in(&fin);
    in.readLine();  //don't read drive's name

    int linenum = getBackInDir(&in, ui, ui->listWidget->currentItem()->text());  //find file position in txt

    in.seek(0);

    QFile fout("dirdata/temp.txt");
    fout.open(QIODevice::WriteOnly);
    QTextStream out(&fout);
    int i = 0;

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList temp = line.split(' ');
        i++;
        if (i == linenum)
        {
            QString notes = temp.at(temp.size() - 1); //get old user notes
            notes.remove(0, 1);
            notes.chop(1); //remove brackets
            QString text = "";
            while (text == "") //get new notes
            {
               text = QInputDialog::getText(0, "Drive",
                                               "Enter " + ui->listWidget->currentItem()->text() + " notes:",
                                               QLineEdit::Normal, notes); //set notes
            }
            text = '\"' + text + '\"';
            temp.pop_back();
            temp << text; //set them back in list
            line = temp.join(' ');
        }
        out << line << '\n';
    }
    fin.close();
    fout.close();
    QFile::remove(currfile);
    QFile::rename("dirdata/temp.txt", currfile);
}

void Widget::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if (ui->listWidget->item(0)->text() == ".." && (item->text() != ".."))
        ui->pushButton_3->setEnabled(true);
    else
        ui->pushButton_3->setEnabled(false);
}
