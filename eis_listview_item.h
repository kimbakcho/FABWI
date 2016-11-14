#ifndef EIS_LISTVIEW_ITEM_H
#define EIS_LISTVIEW_ITEM_H

#include <QWidget>
#include <btextedit.h>
#include <QStandardItem>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>
#include <QSqlError>
#include <QDebug>
#include <QFtp>
#include <QProgressDialog>
#include <QFileDialog>
#include <eis_big_view.h>
#include <global_define.h>
#include <QPrinter>
#include <QPrintDialog>

namespace Ui {
class EIS_listview_item;
}

class EIS_listview_item : public QWidget
{
    Q_OBJECT

public:
    explicit EIS_listview_item(QWidget *parent = 0);
    EIS_listview_item(QString doc_data,QWidget *parent);
    BTextEdit *content_edit;
    QStandardItemModel *attach_list_model;
    QFont mainfont;
    QSqlDatabase db ;
    int doc_number;
    QString server_ip;
    QString db_port;
    QString ftp_port;
    QFtp *ftp;
    QProgressDialog *progressdialog;
    QEventLoop loop;
    QEventLoop listwaitloop;
    Ui::EIS_listview_item *ui;
    QString img_download_file;

    ~EIS_listview_item();

public slots:
    void ftpCommandFinished(int commandId, bool error);

    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);
    void ftp_listInfo(QUrlInfo urlInfo);

    void ftp_rawCommandReply(int a, QString data);



private slots:
    void on_select_team_currentIndexChanged(const QString &arg1);

    void on_select_process_currentIndexChanged(const QString &arg1);

    void on_select_facilities_currentIndexChanged(const QString &arg1);


    void on_font_type_currentTextChanged(const QString &arg1);

    void on_fontsize_editingFinished();

    void on_attach_listview_doubleClicked(const QModelIndex &index);

    void on_modify_button_clicked();

    void on_total_view_content_clicked();

    void on_print_button_clicked();

    void on_table_save_histroy_cellDoubleClicked(int row, int column);

private:

};

#endif // EIS_LISTVIEW_ITEM_H
