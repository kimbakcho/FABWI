#include "hanile_log_main.h"
#include "ui_hanile_log_main.h"

hanile_log_main::hanile_log_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hanile_log_main)
{
    ui->setupUi(this);
    QHeaderView *tempheader;
    tempheader = ui->item_table->horizontalHeader();
    tempheader->resizeSection(3,150); //제목 사이즈 변경
    tempheader->resizeSection(2,50); //시간 사이즈 변경
    tempheader->resizeSection(1,150); //num 사이즈 변경
    tempheader->resizeSection(0,150); //체크박스 사이즈 변경
    main_chart = new hanile_log_err_chart();

    main_chart->setAnimationOptions(QChart::SeriesAnimations);

    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MM-dd HH:mm:ss");
    axisX->setTitleText("Date");
    main_chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("value");
    main_chart->addAxis(axisY, Qt::AlignLeft);

    main_chartview = new hanile_log_chartview(main_chart);
    main_chartview->setRenderHint(QPainter::Antialiasing);

    ui->main_log_layout->addWidget(main_chartview);


}

hanile_log_main::~hanile_log_main()
{
    delete ui;
}

void hanile_log_main::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void hanile_log_main::dropEvent(QDropEvent *event)
{
    QMimeData *source = (QMimeData *)event->mimeData();
    if(source->hasUrls()){
        QList<QUrl> urls = source->urls();
        int temp_beforecount = item_list.count();
        for(int i=0;i<urls.size();i++){
            QFileInfo fileinfo(urls.at(i).toLocalFile());
            if(fileinfo.isDir()){
                QDir tempdir(urls.at(i).toLocalFile());
                QFileInfoList enrtylist = tempdir.entryInfoList(QDir::Files);
                for(int j=0;j<enrtylist.count();j++){
                    hanile_log_item *item = new hanile_log_item();
                    item->setFile_path(enrtylist.at(j).absoluteFilePath());
                    item->file_read();
                    item_list.append(item);
                }
            }else {
                    hanile_log_item *item = new hanile_log_item();
                    item->setFile_path(urls.at(i).toLocalFile());
                    item->file_read();
                    item_list.append(item);
            }
        }

        hanile_log_item *temp =0;
        for(int i=0;i<item_list.size();i++){
            for(int j=0;j<item_list.size()-1;j++){
                int timediff = item_list.at(j)->item_list.at(0)->date_time.msecsTo(item_list.at(j+1)->item_list.at(0)->date_time);
                if(timediff<=0){
                    temp = item_list.at(j);
                    item_list.replace(j,item_list.at(j+1));
                    item_list.replace(j+1,temp);
                }
            }
        }
        int before_row_count  = ui->item_table->rowCount();

        int temp_after_count = item_list.count();
        for(int i=0;i<temp_after_count;i++){
            //qDebug()<<item_list.at(i)->item_list.count();
            for(int j=0;j<item_list.at(i)->item_list.count();j++){
                int count = ui->item_table->rowCount();
                ui->item_table->insertRow(count);
                QLabel *label = item_list.at(i)->item_list.at(j)->getLA_filename();
                //qDebug()<<label->text();
                ui->item_table->setCellWidget(count,0,item_list.at(i)->item_list.at(j)->getLA_filename());
                ui->item_table->setCellWidget(count,1,item_list.at(i)->item_list.at(j)->getLA_filetime());
                ui->item_table->setCellWidget(count,2,item_list.at(i)->item_list.at(j)->getLA_layernumber());
                ui->item_table->setCellWidget(count,3,item_list.at(i)->item_list.at(j)->getLA_Matname());
                ui->item_table->setCellWidget(count,4,item_list.at(i)->item_list.at(j)->getLA_testcheck());
                int for_entpoint_count = item_list.at(i)->item_list.count();
                if(j==for_entpoint_count-1){
                    item_list.at(i)->item_list.at(j)->setData_end_point(item_list.at(i)->getSourceline_list().count());
                }else if(j<for_entpoint_count-1){
                    item_list.at(i)->item_list.at(j)->setData_end_point(item_list.at(i)->item_list.at(j)->getData_start_point()-2);
                }
            }
        }
        for(int i=0;i<before_row_count;i++){
            ui->item_table->removeRow(0);
        }

    }

}

void hanile_log_main::on_MCB_power_toggled(bool checked)
{


    QLineSeries *serise = new QLineSeries();
    QList<QAbstractSeries *> seriselist =  main_chart->series();
    for(int i=0;i<seriselist.count();i++){
        QLineSeries *tempserise = (QLineSeries *)seriselist.at(i);
        if(tempserise->name()=="Power"){
            main_chart->removeSeries(tempserise);
        }
    }
    if(!checked){
        return ;
    }else {
        main_chart->removeAxis(axisX);
        main_chart->removeAxis(axisY);
    }
    for(int i=0;i<item_list.count();i++){
        QStringList linelistdata = item_list.at(i)->getSourceline_list();
        for(int j=0;j<item_list.at(i)->item_list.count();j++){
           int datastartpoint = item_list.at(i)->item_list.at(j)->getData_start_point();
           int dataendpoint = item_list.at(i)->item_list.at(j)->getData_end_point();
           bool first_find = false;
           double average_power =0 ;
           double average_count = 0 ;
           for(int k=datastartpoint;k<dataendpoint;k++){
                QString linedata = linelistdata.at(k);
                QStringList linesplite = linedata.split(",");
                int position = 0;
                if(ui->MCB_standard->currentText()=="Rate"){
                    position = RATE;
                }
                QString temp_line = linesplite.at(position);
                double temp_data1 = temp_line.toDouble();
                double temp_data2 = ui->LE_standard_value->text().toDouble();
                if(temp_data1>=temp_data2 && !first_find){
//                      qDebug()<<"k = "<<k;
//                      qDebug()<<"TIME  = "<<((k - 7)*200)+200;
//                      qDebug()<<"Rate = "<<temp_data1;
                      first_find = true;
                      k = k+((ui->LE_standard_time->text().toInt()*1000)/200);
                }
                if(first_find && temp_data1 !=0){
                    average_count = average_count +1;
                    average_power += temp_data1;
                }
           }
           qDebug()<<"time = "<<item_list.at(i)->item_list.at(j)->getLA_filetime()->text();
           if(average_count == 0){
                item_list.at(i)->item_list.at(j)->getLA_testcheck()->setText("test");
           }else {
                double avr_value= average_power/average_count;
                serise->append(item_list.at(i)->item_list.at(j)->date_time.toMSecsSinceEpoch(),avr_value);
                qDebug()<<"average = "<<average_power/average_count;
           }
        }
    }

    main_chart->addSeries(serise);

    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MM-dd HH:mm:ss");
    axisX->setTitleText("Date");
    main_chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText("value");
    main_chart->addAxis(axisY, Qt::AlignLeft);

    serise->setName("Power");
    serise->attachAxis(axisX);
    serise->attachAxis(axisY);
    serise->setPointsVisible(true);

    serise->setPointLabelsFormat("(@xtimePoint,@yPoint)");

}

void hanile_log_main::on_MZ_reset_btn_clicked()
{
    main_chart->zoomReset();
}

void hanile_log_main::on_MCB_rate_toggled(bool checked)
{
    QLineSeries *serise = new QLineSeries();
    QList<QAbstractSeries *> seriselist =  main_chart->series();
    for(int i=0;i<seriselist.count();i++){
        QLineSeries *tempserise = (QLineSeries *)seriselist.at(i);
        if(tempserise->name()=="Rate"){
            main_chart->removeSeries(tempserise);
        }
    }
    if(!checked){
        return ;
    }else {
        main_chart->removeAxis(axisX);
        main_chart->removeAxis(axisY);
    }
    for(int i=0;i<item_list.count();i++){
        QStringList linelistdata = item_list.at(i)->getSourceline_list();
        for(int j=0;j<item_list.at(i)->item_list.count();j++){
           int datastartpoint = item_list.at(i)->item_list.at(j)->getData_start_point();
           int dataendpoint = item_list.at(i)->item_list.at(j)->getData_end_point();
           bool first_find = false;
           double average_power =0 ;
           double average_count = 0 ;
           for(int k=datastartpoint;k<dataendpoint;k++){
                QString linedata = linelistdata.at(k);
                QStringList linesplite = linedata.split(",");
                int position = 0;
                if(ui->MCB_standard->currentText()=="Rate"){
                    position = RATE;
                }
                QString temp_line = linesplite.at(position);
                double temp_data1 = temp_line.toDouble();
                double temp_data2 = ui->LE_standard_value->text().toDouble();
                if(temp_data1>=temp_data2 && !first_find){
//                      qDebug()<<"k = "<<k;
//                      qDebug()<<"TIME  = "<<((k - 7)*200)+200;
//                      qDebug()<<"Rate = "<<temp_data1;
                      first_find = true;
                      k = k+((ui->LE_standard_time->text().toInt()*1000)/200);
                }
                if(first_find && temp_data1 !=0){
                    average_count = average_count +1;
                    average_power += temp_data1;
                }
           }
           qDebug()<<"time = "<<item_list.at(i)->item_list.at(j)->getLA_filetime()->text();
           if(average_count == 0){
                item_list.at(i)->item_list.at(j)->getLA_testcheck()->setText("test");
           }else {
                double avr_value= average_power/average_count;
                serise->append(item_list.at(i)->item_list.at(j)->date_time.toMSecsSinceEpoch(),avr_value);
                qDebug()<<"average = "<<average_power/average_count;
           }
        }
    }

    main_chart->addSeries(serise);

    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("MM-dd HH:mm:ss");
    axisX->setTitleText("Date");
    main_chart->addAxis(axisX, Qt::AlignBottom);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText("value");
    main_chart->addAxis(axisY, Qt::AlignLeft);

    serise->setName("Rate");
    serise->attachAxis(axisX);
    serise->attachAxis(axisY);
    serise->setPointsVisible(true);

    serise->setPointLabelsFormat("(@xtimePoint,@yPoint)");
}
