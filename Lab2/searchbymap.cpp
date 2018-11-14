#include "searchbymap.h"
#include "ui_searchbymap.h"

QMap<QString, QString> getMetadata(QString line)
{
    QMap<QString, QString> result;
    while (line.size() != 0)
    {
        line.remove(0, line.indexOf('"') + 1); //remove a spacebar or file name and starting " of a key
        QString key = line.mid(0, line.indexOf('"')); //get key
        line.remove(0, line.indexOf('"') + 3); //remove key, ending " of a key, a space bar, and starting " of a value
        QString value = line.mid(0, line.indexOf('"')); //get value
        line.remove(0, line.indexOf('"') + 1); //remove value and ending " of a value
        result[key] = value;
    }
    return result;
}

SearchByMap::SearchByMap(QWidget *parent, QMap<QString, QString> metadata, bool mode) : //if mode = true the search is for 1 key-value
    QDialog(parent),
    ui(new Ui::SearchByMap)
{
    ui->setupUi(this);
    QDirIterator it(QDir::currentPath() + "/dirdata", QStringList() << "*.txt", QDir::Files);
    while (it.hasNext())
    {
        it.next(); //iterate through available drives

        QFile fin(it.filePath());
        fin.open(QIODevice::ReadOnly);
        QTextStream in(&fin);

        in.readLine(); //skip hard drive sector

        QString path = it.fileName();
        path.chop(4); //remove .txt suffix
        path = path + "/";

        int depth = 0;
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QMap<QString, QString> compare;
            if (!metadata.contains("Name"))
                compare = getMetadata(line); //get metadata of a file

            int j = 0;
            while (line[0] == ' ') //count depth of the line
            {
                j++;
                line.remove(0, 1);
            }
            line = line.remove(line.indexOf("\"") - 1, line.size()); //chop off metadata

            if (metadata.contains("Name"))
                    compare["Name"] = line.mid(0, line.indexOf(".")); //get file name without suffix

            while (depth >= j)
            {
                path.chop(1); //remove the /
                path.remove(path.lastIndexOf("/") + 1, path.size()); //chop off last file
                depth--; //for each depth lower
            }
            path = path + line + "/";
            depth++; //one extra depth-- was needed, also covers j == depth + 1
            QMapIterator<QString, QString> i(compare);
            bool equal = true;
            while (i.hasNext()) //compare QMaps
            {
                i.next();
                if (mode)
                {
                    if (metadata.contains(i.key()) && metadata.value(i.key()) != i.value())
                    {
                        equal = false;
                        qDebug() << i.key() << ' ' << i.value();
                        break; //data did not match, go to the next line
                    }
                }
                else
                {
                    if (i.key() == "User notes" || i.key() == "Created") //skip user notes and cretion date
                        continue;
                    if (!metadata.contains(i.key()) || metadata.value(i.key()) != i.value())
                    {
                        equal = false;
                        break; //data did not match, go to the next line
                    }
                }
            }
            if (equal)
                ui->listWidget->addItem(path); //data matched, show on the list
        }
    }
}

SearchByMap::~SearchByMap()
{
    delete ui;
}

void SearchByMap::on_pushButton_clicked()
{
    close();
}
