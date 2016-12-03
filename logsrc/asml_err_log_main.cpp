#include "asml_err_log_main.h"
#include "ui_asml_err_log_main.h"

ASML_err_log_main::ASML_err_log_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ASML_err_log_main)
{
    ui->setupUi(this);
    QHeaderView *tempheader;
    tempheader = ui->bar_list_view->horizontalHeader();
    tempheader->resizeSection(3,50);
    tempheader->resizeSection(2,200);
    tempheader->resizeSection(1,25);
    tempheader->resizeSection(0,25);

    bar_chart = new ASML_err_log_chart();
    bar_chart->setObjectName("asml_err_bar_cahrt");
    bar_chart->setAnimationOptions(QChart::SeriesAnimations);
    bar_chart->setVisible(true);

    bar_chart_view = new ASML_err_log_chartview(bar_chart);
    bar_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->layout_barchart->addWidget(bar_chart_view);

    time_chart = new QChart();
    time_chart->setObjectName("asml_err_line_chart");
    time_chart->setAnimationOptions(QChart::SeriesAnimations);
    lineseries = new QLineSeries();
    time_chart->addSeries(lineseries);
    time_chart->setVisible(true);

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

    time_chart_view = new ASML_err_log_chartview (time_chart);
    time_chart_view->setRenderHint(QPainter::Antialiasing);

    ui->time_chart_layout->addWidget(time_chart_view);
    connect(time_chart_view,SIGNAL(move_value(QPointF)),this,SLOT(slot_move_point(QPointF)));
    series=0;
}

void ASML_err_log_main::analysor(QStringList dataline)
{
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

        QRegularExpression patten;
        patten.setPattern("\\S+");
        QRegularExpressionMatchIterator iter = patten.globalMatch(temp_line_data);
        QStringList captured_data;
        while(iter.hasNext()){
            captured_data<<iter.next().captured();
        }
        QString tempdata = temp_line_data;
        if(tempdata.indexOf("FT")>=0){
            continue;
        }
        if(captured_data.size()>1){
            if((captured_data.at(0)!= "ERROR:") || (captured_data.size() >= 3)){
                continue;
            }
        }else {
            continue;
        }
        QString err_txt = temp_line_data;
        err_txt = err_txt.replace("\n","");

        QString temp_line_data1 = dataline.at(i-1);
        QRegularExpression patten1;
        patten1.setPattern("\\S+");
        QRegularExpressionMatchIterator iter1 = patten1.globalMatch(temp_line_data1);
        QStringList captured_data1;
        while(iter1.hasNext()){
            captured_data1<<iter1.next().captured();
        }
        QString match_str = captured_data1.at(0);

        int year = ((QString)match_str.split('/').at(2)).toInt();
        int month = ((QString)match_str.split('/').at(0)).toInt();
        int day = ((QString)match_str.split('/').at(1)).toInt();
        match_str = captured_data1.at(1);
        int hour = ((QString)match_str.split(':').at(0)).toInt();
        int min = ((QString)match_str.split(':').at(1)).toInt();
        int sec = ((QString)match_str.split(':').at(2)).toInt();
        QDateTime temp_datetime;
        temp_datetime.setDate(QDate(year,month,day));
        temp_datetime.setTime(QTime(hour,min,sec));

        bool isexist = false;
        for(int j=0;j<datalist.size();j++){
            if(datalist.at(j)->data == err_txt){
                isexist = true;
                datalist.at(j)->count = datalist.at(j)->count+1;
                break;
            }
        }
        if(!isexist){
            datalist.append(new data_infrom_1(err_txt,1,dataline.at(i+1)));
        }
    }
    data_infrom_1 *temp =0;
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
        ASML_err_log_list_item *item = new ASML_err_log_list_item(datalist.at(i)->data);
        item->set_dataname(datalist.at(i)->data);
        item->set_count_data(QString("%1").arg(datalist.at(i)->count));
        item->getBar_data()->append(datalist.at(i)->count);
        item->setExplan(datalist.at(i)->error);
        connect(item,SIGNAL(sig_hover_change(QString,QString)),this,SLOT(slot_barset_hover_change(QString,QString)));
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

void ASML_err_log_main::analysor2()
{

}

ASML_err_log_main::~ASML_err_log_main()
{
    delete ui;
}

void ASML_err_log_main::slot_barset_hover_change(QString str,QString error)
{
    ui->bar_chart_hover->setText(str);
    ui->bar_error->setText(error);
}

void ASML_err_log_main::on_bar_chart_zoomreset_btn_clicked()
{
    bar_chart->zoomReset();
}

void ASML_err_log_main::slot_toggle(bool data)
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

void ASML_err_log_main::on_bar_list_view_cellClicked(int row, int column)
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

        QRegularExpression patten;
        patten.setPattern("\\S+");
        QRegularExpressionMatchIterator iter = patten.globalMatch(temp_line_data);
        QStringList captured_data;
        while(iter.hasNext()){
            captured_data<<iter.next().captured();
        }
        QString tempdata = temp_line_data;
        if(tempdata.indexOf("FT")>=0){
            continue;
        }
        if(captured_data.size()>1){
            QString tempdata3 = captured_data.at(1);
            if((captured_data.at(0)!= "ERROR:") || (captured_data.size() >= 3) ){
                continue;
            }
        }else {
            continue;
        }
        QString err_txt = temp_line_data;
        err_txt = err_txt.replace("\n","");
        QString temp_line_data1 = sourceline_list.at(i-1);
        QRegularExpression patten1;
        patten1.setPattern("\\S+");
        QRegularExpressionMatchIterator iter1 = patten1.globalMatch(temp_line_data1);
        QStringList captured_data1;
        while(iter1.hasNext()){
            captured_data1<<iter1.next().captured();
        }
        QString match_str = captured_data1.at(0);

        int year = ((QString)match_str.split('/').at(2)).toInt();
        int month = ((QString)match_str.split('/').at(0)).toInt();
        int day = ((QString)match_str.split('/').at(1)).toInt();
        match_str = captured_data1.at(1);
        int hour = ((QString)match_str.split(':').at(0)).toInt();
        int min = ((QString)match_str.split(':').at(1)).toInt();
        int sec = ((QString)match_str.split(':').at(2)).toInt();
        QDateTime temp_datetime;
        temp_datetime.setDate(QDate(year,month,day));
        temp_datetime.setTime(QTime(hour,min,sec));

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

void ASML_err_log_main::on_line_chart_zoomreset_btn_clicked()
{
    time_chart->zoomReset();
}

void ASML_err_log_main::on_show_line_data_btn_clicked()
{
    if(lineseries->pointLabelsVisible()){
        lineseries->setPointLabelsVisible(false);
    }else {
        lineseries->setPointLabelsVisible(true);
    }
}

void ASML_err_log_main::slot_move_point(QPointF point)
{
    qint64 x = point.x();
    QDateTime datetime;
    datetime.setMSecsSinceEpoch(x);

    ui->X_time_value->setText(datetime.toString("MM-dd HH:mm:ss"));
    ui->Y_value->setText(QString("%1").arg(point.y()));
}

void ASML_err_log_main::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void ASML_err_log_main::dropEvent(QDropEvent *event)
{
    QMimeData *source = (QMimeData *)event->mimeData();
    QString file_name;
    sourceline_list.clear();
    if(source->hasUrls()){

        QList<QUrl> urls = source->urls();
        QString file_path = urls.at(0).toString(QUrl::PreferLocalFile);
        file_name = file_path.split("/").last();
        file_names<<file_name;
        QFile source_file(file_path);
        source_file.open(QIODevice::ReadOnly | QIODevice::Text);

        while(!source_file.atEnd())
        {
            sourceline_list<<source_file.readLine();
        }

        analysor(sourceline_list);
        source_file.close();
    }
}
