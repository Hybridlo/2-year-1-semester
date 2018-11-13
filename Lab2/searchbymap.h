#ifndef SEARCHBYMAP_H
#define SEARCHBYMAP_H

#include <QDialog>
#include <QMap>
#include <QMapIterator>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QDebug> //remove later

namespace Ui {
class SearchByMap;
}

QMap<QString, QString> getMetadata(QString line);

class SearchByMap : public QDialog
{
    Q_OBJECT

public:
    explicit SearchByMap(QWidget *parent = nullptr, QMap<QString, QString> metadata = QMap<QString, QString>());
    ~SearchByMap();

private slots:
    void on_pushButton_clicked();

private:
    Ui::SearchByMap *ui;
};

#endif // SEARCHBYMAP_H
