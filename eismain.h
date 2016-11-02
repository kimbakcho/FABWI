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
#include <QTimer>
#include <QFtp>
#include <QProgressDialog>
#include <QMutex>
#include <QThread>
#include <QEventLoop>
#include <QFileDialog>
#include <QStandardItemModel>
#include <eis_big_view.h>
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
    QSqlDatabase db ;
    QTimer time_update;
    BTextEdit *phenomenon_edit;
    BTextEdit *cause_edit;
    BTextEdit *current_action_edit;
    BTextEdit *current_lot_action_edit;
    BTextEdit *change_master_sheet_edit;
    BTextEdit *next_shift_edit;
    BTextEdit *part_change_edit;
    BTextEdit *after_lot_monitering_edit;
    QFtp *ftp;
    QString server_ip;
    QString db_port;
    QString ftp_port;
    QByteArray ftp_data;
    QProgressDialog *progressdialog;
    QStandardItemModel *attach_list_model;
    QFont mainfont;

    QMutex mutex;
    QEventLoop loop;
    QEventLoop listwaitloop;
    Ui::EISmain *ui;
    ~EISmain();

public slots:
    void on_tableWidget_2_clicked(const QModelIndex &index);

    void on_tableWidget_2_itemClicked(QTableWidgetItem *item);

    void header_click(int data);

    void time_update_out();

    void ftpCommandFinished(int commandId, bool error);

    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);

    void ftp_listInfo(QUrlInfo urlInfo);

    void ftp_rawCommandReply(int a, QString data);



private slots:
    void on_select_process_currentIndexChanged(const QString &arg1);

    void on_select_facilities_currentIndexChanged(const QString &arg1);

    void on_add_button_clicked();

    void on_select_team_currentIndexChanged(const QString &arg1);

    void on_attach_btn_clicked();

    void on_attach_remove_btn_clicked();

    void on_attach_listview_doubleClicked(const QModelIndex &index);

    void on_total_view_phenomenon_clicked();


    void on_fontsize_editingFinished();

    void on_font_type_currentTextChanged(const QString &arg1);

    void on_total_view_cause_clicked();

    void on_total_view_current_action_clicked();

    void on_total_view_current_lot_action_clicked();

    void on_total_view_change_master_sheet_clicked();

private:

};

#endif // EISMAIN_H
