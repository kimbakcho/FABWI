#include "nikon_time_log.h"
#include "ui_nikon_time_log.h"

nikon_time_log::nikon_time_log(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nikon_time_log)
{
    ui->setupUi(this);
    items<<new QStandardItem(tr("filename"));
    filetable_model.setHorizontalHeaderItem(0,new QStandardItem(tr("filename")));
    ui->filelist_table->setModel(&filetable_model);
    if(!db.contains("nikon_err_log")){
           db = QSqlDatabase::addDatabase("QSQLITE","nikon_err_log");
//            QString dbname = qApp->applicationDirPath()+"/logfile/"
//                   +QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss_")
//                   +"nikon_err.db";
           QString dbname = ":memory:";
     qDebug()<<dbname;
     db.setDatabaseName(dbname);
     if(!db.isOpen()){
          if(!db.open()){
               qDebug()<<"false";
           }
        }
    }
    QSqlQuery query(db);
    query.exec("CREATE TABLE [proc_name_history]( "
               "[time] DATETIME, "
               "[name] TEXT); ");

    query.exec("CREATE TABLE [WAFER_CHANGE]( "
        "[name] TEXT, "
        "[start_time] DATETIME, "
        "[endtime] DATETIME, "
        "[diff] TIME) ");
    query.exec("CREATE TABLE [WAFER_ALIGNMENT]( "
                "[name] TEXT, "
                "[start_time] DATETIME, "
                "[endtime] DATETIME, "
                "[diff] TIME) ");
    query.exec("CREATE TABLE [EXPOSURE]( "
                  "[name] TEXT, "
                  "[start_time] DATETIME, "
                  "[endtime] DATETIME, "
                  "[diff] TIME) ");



}

void nikon_time_log::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void nikon_time_log::dropEvent(QDropEvent *event)
{
    QMimeData *source = (QMimeData *)event->mimeData();
    QString file_name;
    if(source->hasUrls()){
        QList<QUrl> urls = source->urls();
        for(int i=0;i<urls.count();i++){
            file_paths<<urls.at(i).toString(QUrl::PreferLocalFile);
            QString file_path = urls.at(i).toString(QUrl::PreferLocalFile);
            file_name = file_path.split("/").last();
            file_paths<<file_name;
            filetable_model.appendRow(new QStandardItem(file_name));
        }
        analysor(file_paths);
    }
}

void nikon_time_log::analysor(QStringList file_paths)
{
    QDateTime current_datetime;
    QString current_datetimestr;
    QSqlQuery query(db);
    QStringList sourceline_list;
    for(int i=0;i<file_paths.count();i++){
        QFile source_file(file_paths.at(i));
        source_file.open(QIODevice::ReadOnly | QIODevice::Text);
        while(!source_file.atEnd())
        {
            QString readline= source_file.readLine();

            if(readline.indexOf("Logging start at")>=0){
                QStringList splitstr = readline.split(" ");
                current_datetimestr = splitstr.at(3)+" "+splitstr.at(4);
                QString datetimedata = splitstr.at(3)+" "+splitstr.at(4);
                current_datetime= change_str_to_datetime(datetimedata);
            }else if(readline.mid(0,2).compare("20")==0){
                QStringList splitstr = readline.split(" ");
                current_datetimestr = splitstr.at(0)+" "+splitstr.at(1);
                QString datetimedata = splitstr.at(0)+" "+splitstr.at(1);
                current_datetime= change_str_to_datetime(datetimedata);
            }else if(readline.indexOf("  'PROC NAME' ")>=0){
                QStringList tempdata = readline.split(" ");
                QString temp_name = tempdata.last();
                temp_name = temp_name.replace("\"","");
                temp_name = temp_name.replace("\n","");
                query.exec(QString("insert into proc_name_history (time,name) values('%1','%2')")
                           .arg(current_datetimestr).arg(temp_name));
            }
            sourceline_list<<readline;
        }
    }
    for(int i=0;i<sourceline_list.count();i++){
        QString readline = sourceline_list.at(i);
        if(readline.indexOf("WAFER CHANGE START")>=0){
            QStringList temp_time = readline.split(" ");
            QString start_time = temp_time.at(0)+" "+temp_time.at(1);
            query.exec(QString("select * from proc_name_history where time <= '%1' order by time desc LIMIT 1").arg(start_time));
            if(query.next()){
                QString name  = query.value("name").toString();
                query.exec(QString("insert into WAFER_CHANGE(name,start_time) values('%1','%2')").arg(name).arg(start_time));
            }
        }else if(readline.indexOf("WAFER ALIGNMENT START")>=0){
            QStringList temp_time = readline.split(" ");
            QString start_time = temp_time.at(0)+" "+temp_time.at(1);
            query.exec(QString("select * from proc_name_history where time <= '%1' order by time desc LIMIT 1").arg(start_time));
            if(query.next()){
                QString name  = query.value("name").toString();
                query.exec(QString("insert into WAFER_ALIGNMENT(name,start_time) values('%1','%2')").arg(name).arg(start_time));
            }
        }else if(readline.indexOf("EXPOSURE START")>=0){
            QStringList temp_time = readline.split(" ");
            QString start_time = temp_time.at(0)+" "+temp_time.at(1);
            query.exec(QString("select * from proc_name_history where time <= '%1' order by time desc LIMIT 1").arg(start_time));
            if(query.next()){
                QString name  = query.value("name").toString();
                query.exec(QString("insert into EXPOSURE(name,start_time) values('%1','%2')").arg(name).arg(start_time));
            }
        }
    }

    for(int i=0;i<sourceline_list.count();i++){
        QString readline = sourceline_list.at(i);
        if(readline.indexOf("WAFER CHANGE END")>=0){
            QStringList temp_time = readline.split(" ");
            QString end_time = temp_time.at(0)+" "+temp_time.at(1);
            query.exec(QString("select * from WAFER_CHANGE where start_time <= '%1' order by start_time desc LIMIT 1").arg(end_time));
            if(query.next()){
                QString start_time  = query.value("start_time").toString();
                QDateTime date_start_time  = query.value("start_time").toDateTime();
                QDateTime date_end_time  = change_str_to_datetime(end_time);
                int ms = date_start_time.msecsTo(date_end_time);
                QTime difftime(0,0,0,0);
                difftime = difftime.addMSecs(ms);
                query.exec(QString("update WAFER_CHANGE set endtime = '%1',diff = '%2' where start_time = '%3' ").arg(end_time).arg(difftime.toString("hh:mm:ss")).arg(start_time));
            }
        }else if(readline.indexOf("WAFER ALIGNMENT END")>=0){
            QStringList temp_time = readline.split(" ");
            QString end_time = temp_time.at(0)+" "+temp_time.at(1);
            query.exec(QString("select * from WAFER_ALIGNMENT where start_time <= '%1' order by start_time desc LIMIT 1").arg(end_time));
            if(query.next()){
                QString start_time  = query.value("start_time").toString();
                QDateTime date_start_time  = query.value("start_time").toDateTime();
                QDateTime date_end_time  = change_str_to_datetime(end_time);
                int ms = date_start_time.msecsTo(date_end_time);
                QTime difftime(0,0,0,0);
                difftime = difftime.addMSecs(ms);
                query.exec(QString("update WAFER_ALIGNMENT set endtime = '%1',diff = '%2' where start_time = '%3' ").arg(end_time).arg(difftime.toString("hh:mm:ss")).arg(start_time));
            }
        }else if(readline.indexOf("EXPOSURE END")>=0){
            QStringList temp_time = readline.split(" ");
            QString end_time = temp_time.at(0)+" "+temp_time.at(1);
            query.exec(QString("select * from EXPOSURE where start_time <= '%1' order by start_time desc LIMIT 1").arg(end_time));
            if(query.next()){
                QString start_time  = query.value("start_time").toString();
                QDateTime date_start_time  = query.value("start_time").toDateTime();
                QDateTime date_end_time  = change_str_to_datetime(end_time);
                int ms = date_start_time.msecsTo(date_end_time);
                QTime difftime(0,0,0,0);
                difftime = difftime.addMSecs(ms);
                query.exec(QString("update EXPOSURE set endtime = '%1',diff = '%2' where start_time = '%3' ").arg(end_time).arg(difftime.toString("hh:mm:ss")).arg(start_time));
            }
        }
    }
    QSqlQueryModel *WAFER_CHANGE_model = new QSqlQueryModel();
    WAFER_CHANGE_model->setQuery("select * from WAFER_CHANGE",db);
    ui->M_WAFER_CHANGE->setModel(WAFER_CHANGE_model);
    WAFER_CHANGE_model->submit();
    query.exec("select * from WAFER_CHANGE");
    QTime WAFER_CHANGE_temptime(0,0,0);
    int WAFER_CHANGE_row_count = 0;
    int WAFER_CHANGE_sec = 0;
    while(query.next()){
        WAFER_CHANGE_row_count++;
        int sec = QTime(0,0,0,0).secsTo(query.value("diff").toTime());
        WAFER_CHANGE_sec += sec;
        WAFER_CHANGE_temptime =WAFER_CHANGE_temptime.addSecs(sec);
    }
//    ui->LA_WAFER_CHANGE_totalsum->setText(WAFER_CHANGE_temptime.toString("hh:mm:ss"));
    double WAFER_CHANGE_value = WAFER_CHANGE_sec/WAFER_CHANGE_row_count;
    ui->LA_WAFER_CHANGE_totalsum->setText(QString("%1").arg(WAFER_CHANGE_value));

    QSqlQueryModel *WAFER_ALIGNMENT_model = new QSqlQueryModel();
    WAFER_ALIGNMENT_model->setQuery("select * from WAFER_ALIGNMENT",db);
    ui->M_WAFER_ALIGNMENT->setModel(WAFER_ALIGNMENT_model);
    WAFER_ALIGNMENT_model->submit();

    query.exec("select * from WAFER_ALIGNMENT");
    QTime WAFER_ALIGNMENT_temptime(0,0,0);
    int WAFER_ALIGNMENT_row_count = 0;
    int WAFER_ALIGNMENT_sec = 0;
    while(query.next()){
        WAFER_ALIGNMENT_row_count++;
        int sec = QTime(0,0,0,0).secsTo(query.value("diff").toTime());
        WAFER_ALIGNMENT_sec += sec;
        WAFER_ALIGNMENT_temptime = WAFER_ALIGNMENT_temptime.addSecs(sec);
    }
    double WAFER_ALIGNMENT_value = WAFER_ALIGNMENT_sec/WAFER_ALIGNMENT_row_count;
    ui->LA_WAFER_ALIGNMENT_totalsum->setText(QString("%1").arg(WAFER_ALIGNMENT_value));
//    ui->LA_WAFER_ALIGNMENT_totalsum->setText(WAFER_ALIGNMENT_temptime.toString("hh:mm:ss"));


    QSqlQueryModel *EXPOSURE_model = new QSqlQueryModel();
    EXPOSURE_model->setQuery("select * from EXPOSURE",db);
    ui->M_EXPOSURE->setModel(EXPOSURE_model);
    EXPOSURE_model->submit();

    query.exec("select * from EXPOSURE");
    QTime EXPOSURE_temptime(0,0,0);
    int EXPOSURE_row_count = 0;
    int EXPOSURE_sec = 0;
    while(query.next()){
        EXPOSURE_row_count++;
        int sec = QTime(0,0,0,0).secsTo(query.value("diff").toTime());
        EXPOSURE_sec += sec;
        EXPOSURE_temptime = EXPOSURE_temptime.addSecs(sec);
    }
    double EXPOSURE_avr_value = EXPOSURE_sec/EXPOSURE_row_count;
    ui->LA_EXPOSURE_totalsum->setText(QString("%1").arg(EXPOSURE_avr_value));
//    ui->LA_EXPOSURE_totalsum->setText(EXPOSURE_temptime.toString("hh:mm:ss"));

//    QTime total_time(0,0,0);
//    total_time = total_time.addSecs(QTime(0,0,0,0).secsTo(WAFER_CHANGE_temptime));
//    total_time = total_time.addSecs(QTime(0,0,0,0).secsTo(WAFER_ALIGNMENT_temptime));
//    total_time = total_time.addSecs(QTime(0,0,0,0).secsTo(EXPOSURE_temptime));
//    ui->LA_M_total_time->setText(total_time.toString("hh:mm:ss"));

}

nikon_time_log::~nikon_time_log()
{
    delete ui;
}

QDateTime nikon_time_log::change_str_to_datetime(QString str)
{
    QStringList src_split = str.split(" ");
    QString temp_data =  src_split.at(0);
    QStringList date_str = temp_data.split("-");
    int year =QString(date_str.at(0)).toInt();
    int month =QString(date_str.at(1)).toInt();
    int day =QString(date_str.at(2)).toInt();
    temp_data = src_split.at(1);
    QStringList time_str = temp_data.split(":");
    int hour = QString(time_str.at(0)).toInt();
    int min = QString(time_str.at(1)).toInt();
    QString tempdata2 = time_str.at(2);
    QStringList tempdata2list = tempdata2.split(".");
    int sec = QString(tempdata2list.at(0)).toInt();
    int ms = QString(tempdata2list.at(1)).toInt();
    QDateTime reslut_datetime;
    reslut_datetime.setDate(QDate(year,month,day));
    reslut_datetime.setTime(QTime(hour,min,sec,ms));
    return reslut_datetime;
}
