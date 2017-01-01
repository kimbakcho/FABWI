#ifndef NIKON_TIME_LOG_H
#define NIKON_TIME_LOG_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlQueryModel>
namespace Ui {
class nikon_time_log;
}

class nikon_time_log : public QWidget
{
    Q_OBJECT

public:
    explicit nikon_time_log(QWidget *parent = 0);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void analysor(QStringList file_paths);
    QStandardItemModel filetable_model;\
    QList<QStandardItem *> items;
    QStringList file_paths;
    QSqlDatabase db;
    ~nikon_time_log();
    QDateTime change_str_to_datetime(QString str);

private:
    Ui::nikon_time_log *ui;
};

#endif // NIKON_TIME_LOG_H
