#ifndef EIS_ALARMLISTVIEW_ITEM_H
#define EIS_ALARMLISTVIEW_ITEM_H

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
#include <QColorDialog>
#include <eis_delete_dialog.h>
#include <QDesktopServices>

namespace Ui {
class eis_alarmlistview_item;
}

class eis_alarmlistview_item : public QWidget
{
    Q_OBJECT

public:
    explicit eis_alarmlistview_item(QWidget *parent = 0);
    eis_alarmlistview_item(QString doc_data, QWidget *parent = 0);
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

    QString img_download_file;

    Ui::eis_alarmlistview_item *ui;
    ~eis_alarmlistview_item();




public slots:
    void ftpCommandFinished(int commandId, bool error);

    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);
    void ftp_listInfo(QUrlInfo urlInfo);

    void ftp_rawCommandReply(int a, QString data);

signals:
    void update_alarm_data();

private slots:
    void on_select_team_currentIndexChanged(const QString &arg1);

    void on_select_process_currentIndexChanged(const QString &arg1);

    void on_select_facilities_currentIndexChanged(const QString &arg1);

    void on_font_type_currentTextChanged(const QString &arg1);

    void on_fontsize_editingFinished();

    void on_attach_listview_doubleClicked(const QModelIndex &index);

    void on_modify_button_clicked();

    void on_total_view_content_clicked();

    void on_table_save_histroy_cellDoubleClicked(int row, int column);

    void on_attach_btn_clicked();

    void on_attach_remove_btn_clicked();

    void on_Bold_btn_clicked();

    void on_underline_btn_clicked();

    void on_color_dialog_clicked();



    void on_delete_btn_clicked();

private:

};

#endif // EIS_ALARMLISTVIEW_ITEM_H
