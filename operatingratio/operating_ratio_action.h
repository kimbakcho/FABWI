#ifndef OPERATING_RATIO_ACTION_H
#define OPERATING_RATIO_ACTION_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <oistystemsoap/oisystemOIWebServiceSoapProxy.h>

namespace Ui {
class operating_ratio_action;
}

class operating_ratio_action : public QWidget
{
    Q_OBJECT

public:
    explicit operating_ratio_action(QWidget *parent = 0);
    QSqlDatabase ms_mesdb;
    OIWebServiceSoapProxy soap_event;
    _ns1__OIWebEquipmentStatus *data;
    std::string siteid;
    std::string equip;
    std::string eventid;
    std::string lang;
    std::string userid;
    std::string resultmessage;
    ~operating_ratio_action();

private slots:

    void on_process_cb_currentIndexChanged(const QString &arg1);

    void on_machinename_cb_currentIndexChanged(const QString &arg1);

    void on_RUN_btn_clicked();

    void on_NONSCHED_btn_clicked();

    void on_ENGR1_btn_clicked();

    void on_SCHDOWN1_btn_clicked();

    void on_SCHDOWN2_btn_clicked();

    void on_USCHDOWN1_btn_clicked();

    void on_USCHDOWN2_btn_clicked();

    void on_USCHDOWN3_btn_clicked();

    void on_USCHDOWN4_btn_clicked();

    void on_WAIT_btn_clicked();

private:
    Ui::operating_ratio_action *ui;
};

#endif // OPERATING_RATIO_ACTION_H
