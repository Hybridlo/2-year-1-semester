#ifndef USERSEARCH_H
#define USERSEARCH_H

#include <QDialog>
#include "searchbymap.h"

namespace Ui {
class UserSearch;
}

class UserSearch : public QDialog
{
    Q_OBJECT

public:
    explicit UserSearch(QWidget *parent = nullptr);
    ~UserSearch();

private slots:
    void on_pushButton_2_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

private:
    Ui::UserSearch *ui;
};

#endif // USERSEARCH_H
