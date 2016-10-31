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
    ~EISmain();

public slots:
    void on_tableWidget_2_clicked(const QModelIndex &index);

    void on_tableWidget_2_itemClicked(QTableWidgetItem *item);

    void header_click(int data);

    void time_update_out();

    void ftpCommandFinished(int commandId, bool error);

    void updateDataTransferProgress(qint64 readBytes,
                                    qint64 totalBytes);

private slots:
    void on_select_process_currentIndexChanged(const QString &arg1);

    void on_select_facilities_currentIndexChanged(const QString &arg1);

    void on_add_button_clicked();

    void on_select_team_currentIndexChanged(const QString &arg1);

private:
    Ui::EISmain *ui;
};

#endif // EISMAIN_H
