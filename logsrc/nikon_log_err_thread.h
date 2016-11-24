#ifndef NIKON_LOG_ERR_THREAD_H
#define NIKON_LOG_ERR_THREAD_H

#include <QObject>
#include <QThread>
#include <QSqlDatabase>
#include <QRegularExpressionMatchIterator>
#include <QRegularExpression>
#include <QSqlQuery>
#include <QDateTime>
class nikon_log_err_thread : public QThread
{
    Q_OBJECT
public:
    nikon_log_err_thread(QStringList data,QSqlDatabase db,QString current_table_name);
    QStringList sourceline_list;
    QSqlDatabase db;
    QString current_table_name;
    void run();
signals:
    void sig_current_count(int count);
};

#endif // NIKON_LOG_ERR_THREAD_H
