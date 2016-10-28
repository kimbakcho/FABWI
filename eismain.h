#ifndef EISMAIN_H
#define EISMAIN_H

#include <QWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <btextedit.h>
#include <QDebug>
#include <QSqlDatabase>
#include <QSettings>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
namespace Ui {
class EISmain;
}

class EISmain : public QWidget
{
    Q_OBJECT

public:
    explicit EISmain(QWidget *parent = 0);
    int doc_number;
    int update_doc_number;
    ~EISmain();

public slots:
    void on_tableWidget_2_clicked(const QModelIndex &index);

    void on_tableWidget_2_itemClicked(QTableWidgetItem *item);

    void header_click(int data);

private:
    Ui::EISmain *ui;
};

#endif // EISMAIN_H
