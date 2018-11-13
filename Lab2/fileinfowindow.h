#ifndef FILEINFOWINDOW_H
#define FILEINFOWINDOW_H

#include <QDialog>
#include "searchbymap.h"
#include <QSignalMapper>

namespace Ui {
class FileInfoWindow;
}

class FileInfoWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FileInfoWindow(QWidget *parent = nullptr, QMap<QString, QString> meta = QMap<QString, QString>());
    QMap<QString, QString> metadata;
    ~FileInfoWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FileInfoWindow *ui;
};

#endif // FILEINFOWINDOW_H
