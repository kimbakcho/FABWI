#include "operating_ratio_action.h"
#include "ui_operating_ratio_action.h"

operating_ratio_action::operating_ratio_action(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::operating_ratio_action)
{
    ui->setupUi(this);
    QString msdb_name = QString("MS_MESDB_OI_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    ms_mesdb = QSqlDatabase::addDatabase("QODBC",msdb_name);
    QString serverinfo = "DRIVER={SQL Server};Server=10.20.10.221;Database=MESDB;Uid=fabview;Port=1433;Pwd=fabview";
    ms_mesdb.setDatabaseName(serverinfo);
    if(!ms_mesdb.contains(msdb_name)){
        if(!ms_mesdb.open()){
            qDebug()<<"fasle";
            qDebug()<<ms_mesdb.lastError().text();
        }else {
            qDebug()<<"open";
        }
    }else {
        ms_mesdb = QSqlDatabase::database(msdb_name);
    }
    data = new _ns1__OIWebEquipmentStatus();
    data->equipmentId = &equip;
    data->siteId = &siteid;
    data->eventId = &eventid;
    data->userId = &userid;
    data->languageId = &lang;
    data->resultMessage = &resultmessage;
    siteid= "WOSF";
    lang= "en-US";
    userid= "PROBE";
    resultmessage= "";
}

operating_ratio_action::~operating_ratio_action()
{
    delete ui;
}


void operating_ratio_action::on_process_cb_currentIndexChanged(const QString &arg1)
{
    QString deposition = tr("deposition");
    QString light = tr("light");
    QString eatching = tr("eatching");
    QString all_probe = tr("ALL probe");
    QSqlQuery query(ms_mesdb);
    ui->machinename_cb->clear();
    if(arg1 == deposition){

        query.exec("SELECT D.EQUIPMENT_GROUP_ID ,B.EQUIPMENT_ID, B.EQUIPMENT_NAME, B.EQUIPMENT_COMMENT, D.EQUIPMENT_GROUP_NAME, B.LAST_EVENT_ID, B.EQUIPMENT_UDF6, B.EQUIPMENT_UDF7 FROM NM_EQUIPMENT (NOLOCK) B INNER JOIN NM_EQUIPMENT_GROUP_RELATIONS (NOLOCK) C ON C.EQUIPMENT_ID = B.EQUIPMENT_ID INNER JOIN NM_EQUIPMENT_GROUPS (NOLOCK) D ON D.EQUIPMENT_GROUP_ID = C.EQUIPMENT_GROUP_ID WHERE B.DELETE_FLAG = 'N' AND B.SITE_ID = 'WOSF' AND D.EQUIPMENT_GROUP_ID = 'MD000' order by EQUIPMENT_NAME ");
        while(query.next()){
            ui->machinename_cb->addItem(query.value("EQUIPMENT_NAME").toString(),QVariant(query.value("EQUIPMENT_ID").toString()));
        }
    }else if(arg1 == light){

        query.exec("SELECT D.EQUIPMENT_GROUP_ID ,B.EQUIPMENT_ID, B.EQUIPMENT_NAME, B.EQUIPMENT_COMMENT, D.EQUIPMENT_GROUP_NAME, B.LAST_EVENT_ID, B.EQUIPMENT_UDF6, B.EQUIPMENT_UDF7 FROM NM_EQUIPMENT (NOLOCK) B INNER JOIN NM_EQUIPMENT_GROUP_RELATIONS (NOLOCK) C ON C.EQUIPMENT_ID = B.EQUIPMENT_ID INNER JOIN NM_EQUIPMENT_GROUPS (NOLOCK) D ON D.EQUIPMENT_GROUP_ID = C.EQUIPMENT_GROUP_ID WHERE B.DELETE_FLAG = 'N' AND B.SITE_ID = 'WOSF' AND D.EQUIPMENT_GROUP_ID = 'ML000' order by EQUIPMENT_NAME ");
        while(query.next()){
            ui->machinename_cb->addItem(query.value("EQUIPMENT_NAME").toString(),QVariant(query.value("EQUIPMENT_ID").toString()));
        }
    }else if(arg1 == eatching){

        query.exec("SELECT D.EQUIPMENT_GROUP_ID ,B.EQUIPMENT_ID, B.EQUIPMENT_NAME, B.EQUIPMENT_COMMENT, D.EQUIPMENT_GROUP_NAME, B.LAST_EVENT_ID, B.EQUIPMENT_UDF6, B.EQUIPMENT_UDF7 FROM NM_EQUIPMENT (NOLOCK) B INNER JOIN NM_EQUIPMENT_GROUP_RELATIONS (NOLOCK) C ON C.EQUIPMENT_ID = B.EQUIPMENT_ID INNER JOIN NM_EQUIPMENT_GROUPS (NOLOCK) D ON D.EQUIPMENT_GROUP_ID = C.EQUIPMENT_GROUP_ID WHERE B.DELETE_FLAG = 'N' AND B.SITE_ID = 'WOSF' AND D.EQUIPMENT_GROUP_ID = 'ME000' order by EQUIPMENT_NAME ");
        while(query.next()){
            ui->machinename_cb->addItem(query.value("EQUIPMENT_NAME").toString(),QVariant(query.value("EQUIPMENT_ID").toString()));
        }
    }else if(arg1 ==all_probe){

        query.exec("SELECT D.EQUIPMENT_GROUP_ID ,B.EQUIPMENT_ID, B.EQUIPMENT_NAME, B.EQUIPMENT_COMMENT, D.EQUIPMENT_GROUP_NAME, B.LAST_EVENT_ID, B.EQUIPMENT_UDF6, B.EQUIPMENT_UDF7 FROM NM_EQUIPMENT (NOLOCK) B INNER JOIN NM_EQUIPMENT_GROUP_RELATIONS (NOLOCK) C ON C.EQUIPMENT_ID = B.EQUIPMENT_ID INNER JOIN NM_EQUIPMENT_GROUPS (NOLOCK) D ON D.EQUIPMENT_GROUP_ID = C.EQUIPMENT_GROUP_ID WHERE B.DELETE_FLAG = 'N' AND B.SITE_ID = 'WOSF' AND D.EQUIPMENT_GROUP_ID = 'MA000' order by EQUIPMENT_NAME ");
        while(query.next()){
            ui->machinename_cb->addItem(query.value("EQUIPMENT_NAME").toString(),QVariant(query.value("EQUIPMENT_ID").toString()));
        }
    }
}

void operating_ratio_action::on_machinename_cb_currentIndexChanged(const QString &arg1)
{
    equip = ui->machinename_cb->currentData().toString().toStdString();
    ui->machinename_LA->setText(arg1);
}

void operating_ratio_action::on_RUN_btn_clicked()
{
    eventid = "RUN";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}

void operating_ratio_action::on_NONSCHED_btn_clicked()
{
    eventid = "NONSCHED";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}


void operating_ratio_action::on_ENGR1_btn_clicked()
{
    eventid = "ENGR1";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}

void operating_ratio_action::on_SCHDOWN1_btn_clicked()
{
    eventid = "SCHDOWN1";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}

void operating_ratio_action::on_SCHDOWN2_btn_clicked()
{
    eventid = "SCHDOWN2";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}

void operating_ratio_action::on_USCHDOWN1_btn_clicked()
{
    eventid = "USCHDOWN1";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}

void operating_ratio_action::on_USCHDOWN2_btn_clicked()
{
    eventid = "USCHDOWN2";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}

void operating_ratio_action::on_USCHDOWN3_btn_clicked()
{
    eventid = "USCHDOWN3";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}

void operating_ratio_action::on_USCHDOWN4_btn_clicked()
{
    eventid = "USCHDOWN4";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}

void operating_ratio_action::on_WAIT_btn_clicked()
{
    eventid = "WAIT";
    _ns1__OIWebEquipmentStatusResponse response;
    soap_event.OIWebEquipmentStatus(data,response);
}
