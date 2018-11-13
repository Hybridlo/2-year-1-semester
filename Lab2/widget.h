#ifndef WIDGET_H
#define WIDGET_H

#include "fileinfowindow.h"
#include <QWidget>
#include <QListWidgetItem>
#include <QString>
#include <QDebug> //remove later
#include <QDir>
#include <QInputDialog>
#include <QUrl>
#include <QDesktopServices>
#include <QDateTime>
#include <QCryptographicHash>
#include <QTextCodec>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
