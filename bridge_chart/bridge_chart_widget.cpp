#include "bridge_chart_widget.h"
#include "ui_bridge_chart_widget.h"

bridge_chart_widget::bridge_chart_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bridge_chart_widget)
{
    ui->setupUi(this);
    QString msdb_name = QString("MS_MESDB_%1").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
    ms_mes_db = QSqlDatabase::addDatabase("QODBC",msdb_name);
    QString serverinfo = "DRIVER={SQL Server};Server=10.20.10.221;Database=MESDB;Uid=fabview;Port=1433;Pwd=fabview";
    ms_mes_db.setDatabaseName(serverinfo);
    if(!ms_mes_db.open()){
        qDebug()<<ms_mes_db.lastError().text();
    }else {

    }
    main_chart = new bridge_chart();
    main_chartview = new bridge_chartview(main_chart);
    ui->chart_layout->addWidget(main_chartview);
    ui->search_end_time->setDateTime(QDateTime::currentDateTime());
    ui->search_start_time->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->row_data_table->setModel(&table_model);


}

bridge_chart_widget::~bridge_chart_widget()
{
    delete ui;
}

void bridge_chart_widget::on_search_btn_clicked()
{
     QSqlQuery main_query(ms_mes_db);

     QString query_txt2 = QString("SELECT a.TX_USER_NAME,a.TX_DTTM,a.MATERIAL_ID,a.LOT_ID,a.ROUTE_ID,"
                                 "a.OPERATION_ID,A.EQUIPMENT_ID,c.EQUIPMENT_NAME,a.VALUE1,a.COLLECTION_ID,"
                                 "b.TARGET_VALUE,b.UPPER_SPEC_LIMIT,b.LOWER_SPEC_LIMIT,b.UPPER_WARN_LIMIT,b.LOWER_WARN_LIMIT  "
                                 "FROM [MESDB].[dbo].[NM_EDC_LOTS] a,[MESDB].[dbo].[NM_COLLECTION_CHARACTERS] b,[MESDB].[dbo].[NM_EQUIPMENT] c "
                                 "where a.COLLECTION_ID = 'Bridge_Thickness'  AND a.CHARACTER_ID = 'Bridge_Thickness' AND  a.CHARACTER_ID = b.CHARACTER_ID AND a.COLLECTION_ID = b.COLLECTION_ID AND a.COLLECTION_VERSION = b.COLLECTION_VERSION AND a.EQUIPMENT_ID = c.EQUIPMENT_ID AND "
                                 "(TX_DTTM between '%1' AND '%2')  order by TX_DTTM asc").arg(ui->search_start_time->dateTime().toString("yyyyMMddhhmmss")).arg(ui->search_end_time->dateTime().toString("yyyyMMddhhmmss"));
     main_query.exec(query_txt2);

     QString query_txt3 = QString("SELECT a.TX_USER_NAME,a.TX_DTTM,a.MATERIAL_ID,a.LOT_ID,a.ROUTE_ID,"
                                 "c.EQUIPMENT_NAME,a.VALUE1,"
                                 "b.TARGET_VALUE,b.UPPER_SPEC_LIMIT,b.LOWER_SPEC_LIMIT  "
                                 "FROM [MESDB].[dbo].[NM_EDC_LOTS] a,[MESDB].[dbo].[NM_COLLECTION_CHARACTERS] b,[MESDB].[dbo].[NM_EQUIPMENT] c "
                                 "where a.COLLECTION_ID = 'Bridge_Thickness'  AND a.CHARACTER_ID = 'Bridge_Thickness' AND  a.CHARACTER_ID = b.CHARACTER_ID AND a.COLLECTION_ID = b.COLLECTION_ID AND a.COLLECTION_VERSION = b.COLLECTION_VERSION AND a.EQUIPMENT_ID = c.EQUIPMENT_ID AND "
                                 "(TX_DTTM between '%1' AND '%2')  order by TX_DTTM desc").arg(ui->search_start_time->dateTime().toString("yyyyMMddhhmmss")).arg(ui->search_end_time->dateTime().toString("yyyyMMddhhmmss"));


     table_model.setQuery(query_txt3,ms_mes_db);
     table_model.submit();

     table_model.setHeaderData(0,Qt::Horizontal,tr("name"));
     table_model.setHeaderData(1,Qt::Horizontal,tr("Time"));
     table_model.setHeaderData(5,Qt::Horizontal,tr("machine_name"));
     table_model.setHeaderData(6,Qt::Horizontal,tr("value1"));
     table_model.setHeaderData(7,Qt::Horizontal,tr("CL"));
     table_model.setHeaderData(8,Qt::Horizontal,tr("USL"));
     table_model.setHeaderData(9,Qt::Horizontal,tr("LSL"));
     ui->row_data_table->horizontalHeader()->resizeSection(1,120);

     main_chart->removeAllSeries();
     if(main_chart->axisX() != NULL){
         main_chart->removeAxis(main_chart->axisX());
     }
     if(main_chart->axisY() != NULL){
         main_chart->removeAxis(main_chart->axisY());
     }
     QPen temp_pen;
     value_series = new QLineSeries;
     value_series->setName("value");
     value_series->setColor(QColor("#141414"));
     temp_pen = value_series->pen();
     temp_pen.setWidthF(2);
     value_series->setPen(temp_pen);
     value_series->setPointLabelsFormat("(@xtimePoint,@yPoint)");

     CL_series = new QLineSeries;
     CL_series->setColor(QColor("#47ff53"));
     CL_series->setName("CL");

     USL_series[0] = new QLineSeries;
     USL_series[0]->setName("A");
     USL_series[0]->setColor(QColor("#ff0004"));
     USL_series[1] = new QLineSeries;
     USL_series[1]->setName("B");
     USL_series[1]->setColor(QColor("#ff636d"));
     USL_series[2] = new QLineSeries;
     USL_series[2]->setName("C");
     USL_series[2]->setColor(QColor("#ffc2cb"));
     LSL_series[0] = new QLineSeries;
     LSL_series[0]->setName("A");
     LSL_series[0]->setColor(QColor("#ff0004"));
     LSL_series[1] = new QLineSeries;
     LSL_series[1]->setName("B");
     LSL_series[1]->setColor(QColor("#ff636d"));
     LSL_series[2] = new QLineSeries;
     LSL_series[2]->setName("C");
     LSL_series[2]->setColor(QColor("#ffc2cb"));
     qreal setp = 0 ;
     value_series->setPointsVisible(true);
     while(main_query.next()){
         QDateTime search_time = QDateTime::fromString(main_query.value("TX_DTTM").toString(),QString("yyyyMMddHHmmss"));

         value_series->append(search_time.toMSecsSinceEpoch(),main_query.value("VALUE1").toInt());
         int diff = main_query.value("UPPER_SPEC_LIMIT").toInt() - main_query.value("LOWER_SPEC_LIMIT").toInt();
         setp =  (qreal)diff/6.0;
         USL_series[0]->append(search_time.toMSecsSinceEpoch(),main_query.value("UPPER_SPEC_LIMIT").toInt());
         USL_series[1]->append(search_time.toMSecsSinceEpoch(),main_query.value("UPPER_SPEC_LIMIT").toDouble()-setp*1);
         USL_series[2]->append(search_time.toMSecsSinceEpoch(),main_query.value("UPPER_SPEC_LIMIT").toDouble()-setp*2);
         CL_series->append(search_time.toMSecsSinceEpoch(),main_query.value("TARGET_VALUE").toInt());
         LSL_series[2]->append(search_time.toMSecsSinceEpoch(),main_query.value("LOWER_SPEC_LIMIT").toInt()+setp*2);
         LSL_series[1]->append(search_time.toMSecsSinceEpoch(),main_query.value("LOWER_SPEC_LIMIT").toInt()+setp*1);
         LSL_series[0]->append(search_time.toMSecsSinceEpoch(),main_query.value("LOWER_SPEC_LIMIT").toInt());
     }
     main_chart->addSeries(value_series);
      main_chart->addSeries(USL_series[0]);
      main_chart->addSeries(USL_series[1]);
      main_chart->addSeries(USL_series[2]);
      main_chart->addSeries(LSL_series[0]);
      main_chart->addSeries(LSL_series[1]);
      main_chart->addSeries(LSL_series[2]);
      main_chart->addSeries(CL_series);
      axisX = new QDateTimeAxis;
      axisX->setTickCount(9);
      axisX->setFormat("MM-dd HH:mm:ss");
      axisX->setTitleText("Date");
      axisX->setVisible(true);
      axisX->setTitleVisible(true);
//      axisX->setRange(search_start_time,search_end_time);
      main_chart->addAxis(axisX, Qt::AlignBottom);
      axisY = new QValueAxis;
      axisY->setRange(LSL_series[0]->at(0).y()-(setp),USL_series[0]->at(0).y()+(setp));
      main_chart->addAxis(axisY,Qt::AlignLeft);
      value_series->attachAxis(axisX);
      value_series->attachAxis(axisY);
      USL_series[0]->attachAxis(axisX);
      USL_series[0]->attachAxis(axisY);
      USL_series[1]->attachAxis(axisX);
      USL_series[1]->attachAxis(axisY);
      USL_series[2]->attachAxis(axisX);
      USL_series[2]->attachAxis(axisY);
      LSL_series[0]->attachAxis(axisX);
      LSL_series[0]->attachAxis(axisY);
      LSL_series[1]->attachAxis(axisX);
      LSL_series[1]->attachAxis(axisY);
      LSL_series[2]->attachAxis(axisX);
      LSL_series[2]->attachAxis(axisY);
      CL_series->attachAxis(axisX);
      CL_series->attachAxis(axisY);


}

void bridge_chart_widget::closeEvent(QCloseEvent *event)
{
    this->deleteLater();
}

void bridge_chart_widget::on_zoom_reset_btn_clicked()
{
    main_chart->zoomReset();
}

void bridge_chart_widget::on_point_visiable_btn_clicked()
{
    if(value_series->pointLabelsVisible()){
        value_series->setPointLabelsVisible(false);
    }else {
        value_series->setPointLabelsVisible(true);
    }
    main_chartview->update();
}
