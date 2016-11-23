#include "nikon_log_err_thread.h"


nikon_log_err_thread::nikon_log_err_thread(QStringList data, QSqlDatabase db, QString current_table_name)
{
    sourceline_list = data;
    this->db = db;
    this->current_table_name = current_table_name;
}

void nikon_log_err_thread::run()
{
    QSqlQuery query(db);
    for(int i=0;i<sourceline_list.size();i++){
        QString temp_line_data = sourceline_list.at(i);
        //qDebug()<<temp_line_data;
        if((temp_line_data.indexOf("Logging start")>=0)
                ||temp_line_data.indexOf("Logging end")>=0){
            continue;
        }

        QRegularExpression patten;
        patten.setPattern("\\S+");
        QRegularExpressionMatchIterator iter = patten.globalMatch(temp_line_data);
        QStringList captured_data;
        while(iter.hasNext()){
            captured_data<<iter.next().captured();
        }

        QString match_str = captured_data.at(0);

        int year = ((QString)match_str.split('-').at(0)).toInt();
        int month = ((QString)match_str.split('-').at(1)).toInt();
        int day = ((QString)match_str.split('-').at(2)).toInt();
        match_str = captured_data.at(1);
        int hour = ((QString)match_str.split(':').at(0)).toInt();
        int min = ((QString)match_str.split(':').at(1)).toInt();
        int sec = ((QString)match_str.split(':').at(2)).toInt();
        int msec = ((QString)match_str.split('.').at(1)).toInt();
        QDateTime temp_datetime;
        temp_datetime.setDate(QDate(year,month,day));
        temp_datetime.setTime(QTime(hour,min,sec,msec));
        QString err_txt = captured_data.last();
        QString query_txt = QString("insert into '%1' ('time','exe','data') values ('%2','%3','%4');")
                .arg(current_table_name)
                .arg(temp_datetime.toString("yyyy-MM-dd HH:mm:ss.zzz"))
                .arg(1)
                .arg(err_txt);

        query.exec(query_txt);
        emit sig_current_count(i);
    }
}
