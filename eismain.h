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
#include <eis_serarch_item.h>
#include <QVector>
#include <eis_list_view.h>
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
    QVector<EIS_serarch_item *> itemlist;
    QStringList list_check_list;

    QMutex mutex;
    QEventLoop loop;
    QEventLoop listwaitloop;
    Ui::EISmain *ui;
    ~EISmain();
    QString Return_search_query();

public slots:

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

    void on_total_view_next_shift_clicked();

    void on_total_view_part_change_clicked();

    void on_total_view_after_lot_monitering_clicked();

    void on_search_select_team_currentIndexChanged(const QString &arg1);

    void on_search_select_process_currentIndexChanged(const QString &arg1);

    void on_search_select_facilities_currentIndexChanged(const QString &arg1);

    void on_search_button_clicked();

    void on_search_listview_cellDoubleClicked(int row, int column);



    void on_total_view_btn_clicked();

private:

};

#endif // EISMAIN_H
