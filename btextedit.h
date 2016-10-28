#ifndef BTEXTEDIT_H
#define BTEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QMimeData>
#include <QDebug>
#include <QTextBrowser>
#include <QFile>
#include <QDir>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QApplication>
#include <QDateTime>
#include <QTime>
#include <QTextTable>
class BTextEdit : public QTextBrowser
{
    Q_OBJECT
public:
    BTextEdit(QWidget *parent = 0);
    BTextEdit(int *doc_number,QWidget *parent = 0);
    bool ispicture(QString source);
    QString outputpath(QString source);
    QString outputfilename(QString source);
    int *doc_number;
private:
    void insertFromMimeData(const QMimeData *source);
public slots:
    void replyFinished(QNetworkReply* reply);
};

#endif // BTEXTEDIT_H
