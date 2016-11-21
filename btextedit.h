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
#include <d_image_size.h>
#include <QMessageBox>
#include <QTextDocumentFragment>
#include <eis_select_copymode_dialog.h>
#define PICTURE_MODE 1
#define TEXT_MODE 2
class BTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    BTextEdit(QWidget *parent = 0);
    BTextEdit(int *doc_number,QWidget *parent = 0);
    bool ispicture(QString source);
    QString outputfilename(QString source);
    int *doc_number;
    QFont mainfont;
    QWidget *parent_widget;
    QTextCharFormat charformat;
    QString tosqlhtml();
    QStringList image_list;
    QStringList getImage_list() const;


    void setImage_list(const QStringList &value);

protected:
    void insertFromMimeData( const QMimeData *source ) ;
public slots:
    void replyFinished(QNetworkReply* reply);
    void b_text_cursor_change();
};

#endif // BTEXTEDIT_H
