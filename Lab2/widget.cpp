#include "widget.h"
#include "ui_widget.h"
#include "fileinfowindow.h"

void initDrives(Ui::Widget *ui)
{
    ui->listWidget->clear();
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
    QString line = in->readLine();
    int i = 0, linenum = 1; //1 for skipped hard drive sector name
    while (!inPrevpath)
    {
        line = in->readLine();
        line.remove(line.indexOf("\"") - 1, line.size()); //remove metadata
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
    return linenum;
}

void addMetadata(QTextStream *out, QString key, QString value)
{
    *out << " \"" << key << "\" \"" << value << "\"";
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    initDrives(ui);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
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
           text = QInputDialog::getText(this, "Drive",
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
            out << newdir;
            addMetadata(&out, "Created", info.created().toString("dd.MM.yyyy HH:mm:ss"));
            addMetadata(&out, "Last modified", info.lastModified().toString("dd.MM.yyyy HH:mm:ss"));

            if (it.fileName().indexOf('.') != -1) //directories have same hash
                addMetadata(&out, "SHA-1 Hash", hash.toHex());
            addMetadata(&out, "User notes", "");
            out << '\n';
        }
        fout.close();
    }
    initDrives(ui);
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
           initDrives(ui);  //show offline drives if got out of some drive
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

    int linenum = getBackInDir(&in, ui, ui->listWidget->currentItem()->text());  //find file position in txt

    in.seek(0); //get back to start of the txt

    QFile fout("dirdata/temp.txt");
    fout.open(QIODevice::WriteOnly);
    QTextStream out(&fout);
    int i = 0;

    while (!in.atEnd())
    {
        QString line = in.readLine();
        i++;
        if (i == linenum)
        {
            QMap<QString, QString> temp = getMetadata(line);
            QString notes = temp.value("User notes"); //get old user notes
            QString text = "";
            while (text == "") //get new notes
            {
               text = QInputDialog::getText(this, "Drive",
                                               "Enter " + ui->listWidget->currentItem()->text() + " notes:",
                                               QLineEdit::Normal, notes); //set notes
            }
            temp["User notes"] = text;
            line = line.mid(0, line.indexOf('"') - 1); //remove old metadata
            QMapIterator<QString, QString> i(temp);
            while (i.hasNext()) {
                i.next();
                line = line + " \"" + i.key() + "\" \"" + i.value() + "\""; //add new metadata
            }
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
     {
         ui->pushButton_3->setEnabled(true);
         ui->pushButton_4->setEnabled(true);
     }
     else
     {
         ui->pushButton_3->setEnabled(false);
         ui->pushButton_4->setEnabled(false);
     }
}

void Widget::on_pushButton_4_clicked()
{
    QFile fin(currfile);
    fin.open(QIODevice::ReadOnly);
    QTextStream in(&fin);

    getBackInDir(&in, ui, ""); //get back in directory where highlighted file is
    QString line, templine;
    while (templine != (QString(' ').repeated(templine.lastIndexOf(" ") + 1) + ui->listWidget->currentItem()->text()))
    {   //looking for that directory
        line = in.readLine();
        templine = line.mid(0, line.indexOf("\"") - 1); //remove metadata
    }   //at the end line = highlighted item with it's metadata
    QMap<QString, QString> metadata = getMetadata(line);  //parse metadata from line
    FileInfoWindow fileInfoWindow(this, metadata);
    fileInfoWindow.exec();
}

void Widget::on_pushButton_5_clicked()
{

}
