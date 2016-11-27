#include "nikon_log_err_main.h"
#include "ui_nikon_log_err_main.h"

nikon_log_err_main::nikon_log_err_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nikon_log_err_main)
{
    ui->setupUi(this);

    QHeaderView *tempheader;
    tempheader = ui->bar_list_view->horizontalHeader();
    tempheader->resizeSection(3,50); //제목 사이즈 변경
    tempheader->resizeSection(2,200); //시간 사이즈 변경
    tempheader->resizeSection(1,25); //num 사이즈 변경
    tempheader->resizeSection(0,25); //체크박스 사이즈 변경


    bar_chart = new nikon_log_err_chart();
    bar_chart->setObjectName("nikon_err_bar_chart");
    bar_chart->setAnimationOptions(QChart::SeriesAnimations);

    bar_chart_view = new nikon_log_err_chartview (bar_chart);
    bar_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->layout_barchart->addWidget(bar_chart_view);

    time_chart = new nikon_log_err_chart();
    time_chart->setObjectName("nikon_err_line_chart");
    time_chart->setAnimationOptions(QChart::SeriesAnimations);
    lineseries = new QLineSeries();
    time_chart->addSeries(lineseries);

    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MM-dd HH:mm:ss");
    axisX->setTitleText("Date");
    time_chart->addAxis(axisX, Qt::AlignBottom);
    lineseries->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("err count");
    time_chart->addAxis(axisY, Qt::AlignLeft);
    lineseries->attachAxis(axisY);


    time_chart_view = new nikon_log_err_chartview (time_chart);
    time_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->time_chart_layout->addWidget(time_chart_view);
    connect(time_chart_view,SIGNAL(move_value(QPointF)),this,SLOT(slot_move_point(QPointF)));
    series=0;



}





nikon_log_err_main::~nikon_log_err_main()
{
    delete ui;
}

void nikon_log_err_main::slot_barset_hover_change(QString str)
{
    ui->bar_chart_hover->setText(str);
}

void nikon_log_err_main::dragEnterEvent(QDragEnterEvent *event)
{
     event->acceptProposedAction();
}

void nikon_log_err_main::dropEvent(QDropEvent *event)
{
    QMimeData *source = (QMimeData *)event->mimeData();
    QString file_name;
    sourceline_list.clear();
    if(source->hasUrls()){
//        QDir dir(qApp->applicationDirPath());
//        if(!dir.exists("logfile")){
//            dir.mkdir("logfile");
//        }
//        if(!db.contains("nikon_err_log")){
//            db = QSqlDatabase::addDatabase("QSQLITE","nikon_err_log");
//            QString dbname = qApp->applicationDirPath()+"/logfile/"
//                    +QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss_")
//                    +"nikon_err.db";
//            qDebug()<<dbname;
//            db.setDatabaseName(dbname);
//            if(!db.isOpen()){
//                if(!db.open()){
//                    qDebug()<<"false";
//                }
//            }
//        }
        QList<QUrl> urls = source->urls();
        QString file_path = urls.at(0).toString(QUrl::PreferLocalFile);
        file_name = file_path.split("/").last();
        file_names<<file_name;

//        QString table_name = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH:mm:ss")+"/"+file_name;
//        current_table_name = table_name;
//        QSqlQuery query(db);
//        QString query_txt = QString("CREATE TABLE [%1]([time] DATETIME,[exe] TEXT,[data] TEXT);").arg(table_name);
//        query.exec(query_txt);
//        current_count_table_name = table_name+"/count_data";
//        query_txt = QString("CREATE TABLE [%1]([data] TEXT,[count] INT);").arg(current_count_table_name);
//        query.exec(query_txt);

        QFile source_file(file_path);
        source_file.open(QIODevice::ReadOnly | QIODevice::Text);

        while(!source_file.atEnd())
        {
            sourceline_list<<source_file.readLine();
        }

        analysor(sourceline_list);
        source_file.close();
//        QProgressDialog *dialog = new QProgressDialog();
//        dialog->setMaximum(sourceline_list.size()-2);
//        nikon_log_err_thread *new_thread = new nikon_log_err_thread(sourceline_list,db,current_table_name);
//        connect(new_thread,SIGNAL(sig_current_count(int)),dialog,SLOT(setValue(int)));
//        new_thread->start();
//        dialog->exec();
//        dialog->deleteLater();
//        new_thread->deleteLater();

//        query.exec(QString("select * from %1 GROUP BY data ").arg(current_table_name));
//        QSqlQuery query_2(db);
//        while(query.next()){
//            query_2.exec(QString("select * from '%1' where data = '%2'").arg(current_table_name).arg(query.value("data").toString()));
//            int count =0;
//            while(query_2.next()){
//                count++;
//            }
//            query_2.exec(QString("insert into '%1' ('data','count') values ('%2',%3)")
//                         .arg(current_count_table_name)
//                         .arg(query.value("data").toString())
//                         .arg(count)
//                         );
//        }

    }

}
void nikon_log_err_main::analysor(QStringList dataline)
{
//    bar_chart->removeSeries(series);
    for(int i=0;i<bar_chart->series().count();i++){
        QBarSeries *temp_series = (QBarSeries *)bar_chart->series().at(i);
        bar_chart->removeSeries(temp_series);
        for(int j=0;j<temp_series->barSets().count();j++){
            QBarSet *tempbar = (QBarSet *)temp_series->barSets().at(i);
            temp_series->remove(tempbar);
        }
    }

    for(int i=0;i<item_list.count();i++){
        item_list.at(i)->deleteLater();
    }
    item_list.clear();

    datalist.clear();

    for(int i=0;i<dataline.size();i++){
        QString temp_line_data = dataline.at(i);
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
        QString err_txt = captured_data.at(10);
        bool isexist = false;
        for(int j=0;j<datalist.size();j++){
            if(datalist.at(j)->data == err_txt){
                isexist = true;
                datalist.at(j)->count = datalist.at(j)->count+1;
                break;
            }
        }
        if(!isexist){
            datalist.append(new data_infrom(err_txt,0));
        }
    }

    data_infrom *temp =0;
    for(int i=0;i<datalist.size();i++){
        for(int j=0;j<datalist.size()-1;j++){
            if(datalist.at(j)->count<=datalist.at(j+1)->count){
                temp = datalist.at(j);
                datalist.replace(j,datalist.at(j+1));
                datalist.replace(j+1,temp);
            }
        }
    }
    series = new QBarSeries();
    for(int i=0;i<datalist.size();i++){
        nikon_log_err_list_item *item = new nikon_log_err_list_item(datalist.at(i)->data);
        item->set_dataname(datalist.at(i)->data);
        item->set_count_data(QString("%1").arg(datalist.at(i)->count));
        item->getBar_data()->append(datalist.at(i)->count);
        connect(item,SIGNAL(sig_hover_change(QString)),this,SLOT(slot_barset_hover_change(QString)));
        connect(item->getCb_check_box(),SIGNAL(toggled(bool)),this,SLOT(slot_toggle(bool)));
        item_list.append(item);
        series->append(item_list.at(i)->getBar_data());
        int count = ui->bar_list_view->rowCount();
        ui->bar_list_view->insertRow(count);
        ui->bar_list_view->setCellWidget(count,0,item->getCb_check_box());
        ui->bar_list_view->setCellWidget(count,1,item->getLA_data_color());
        ui->bar_list_view->setCellWidget(count,2,item->getLA_data_name());
        ui->bar_list_view->setCellWidget(count,3,item->getLA_data_count());
    }
    series->setLabelsVisible(true);

    series->setBarWidth(1);
    series->setLabelsAngle(90);

    bar_chart->addSeries(series);
    bar_chart->legend()->setVisible(false);
    bar_chart->legend()->setAlignment(Qt::AlignLeft);
    QBarCategoryAxis axis;
    axis.append("   ");
    bar_chart->createDefaultAxes();
    bar_chart->setAxisX(&axis, series);

    for(int i=0;i<item_list.count();i++){
        QColor bar_color = item_list.at(i)->bar_data->color();
        item_list.at(i)->getLA_data_color()->setStyleSheet(
                    QString("background-color : %1").arg(bar_color.name()));
    }
}

void nikon_log_err_main::analysor2()
{

}


void nikon_log_err_main::on_bar_chart_zoomreset_btn_clicked()
{
    bar_chart->zoomReset();
}

void nikon_log_err_main::slot_toggle(bool data)
{
    bar_chart->removeSeries(series);
    series = new QBarSeries();
    for(int i=0;i<item_list.count();i++){
        if(item_list.at(i)->getCb_check_box()->isChecked()){
            series->append(item_list.at(i)->getBar_data());
        }else {

        }
    }
    series->setLabelsVisible(true);

    series->setBarWidth(1);
    series->setLabelsAngle(90);

    bar_chart->addSeries(series);
    bar_chart->createDefaultAxes();

}

void nikon_log_err_main::on_bar_list_view_cellClicked(int row, int column)
{
    QLabel *label = (QLabel *)ui->bar_list_view->cellWidget(row,2);
    QLabel *color = (QLabel *)ui->bar_list_view->cellWidget(row,1);
    time_chart->removeSeries(lineseries);
    time_chart->removeAxis(axisX);
    time_chart->removeAxis(axisY);
    QString data_name = label->text();
    QString data_color = color->styleSheet();
    data_color = data_color.split("#").at(1);
    data_color = '#'+data_color;
    lineseries = new QLineSeries();
    int count = 0;
    for(int i=0;i<sourceline_list.count();i++){
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
        QString err_txt = captured_data.at(10);
        if(err_txt == data_name){
            count++;
            lineseries->append(temp_datetime.toMSecsSinceEpoch(),count);
        }
    }



    time_chart->addSeries(lineseries);

    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MM-dd HH:mm:ss");
    axisX->setTitleText("Date");
    time_chart->addAxis(axisX, Qt::AlignBottom);
    lineseries->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("err count");
    time_chart->addAxis(axisY, Qt::AlignLeft);
    lineseries->attachAxis(axisY);

    lineseries->setColor(QColor(data_color));

    lineseries->setPointLabelsFormat("(@xtimePoint,@yPoint)");


    lineseries->setPointsVisible(true);

}

void nikon_log_err_main::on_line_chart_zoomreset_btn_clicked()
{
    time_chart->zoomReset();
}

void nikon_log_err_main::on_show_line_data_btn_clicked()
{
    if(lineseries->pointLabelsVisible()){
        lineseries->setPointLabelsVisible(false);
    }else {
        lineseries->setPointLabelsVisible(true);
    }
}

void nikon_log_err_main::slot_move_point(QPointF point)
{
    qint64 x = point.x();
    QDateTime datetime;
    datetime.setMSecsSinceEpoch(x);

    ui->X_time_value->setText(datetime.toString("MM-dd HH:mm:ss"));
    ui->Y_value->setText(QString("%1").arg(point.y()));
}
