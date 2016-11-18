#include "etching_analysor.h"
#include "ui_etching_analysor.h"

etching_analysor::etching_analysor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::etching_analysor)
{
    ui->setupUi(this);
    line = new QLineSeries();
    chart = new etching_chart_1(line);
    chartview = new etching_chart_1_view(chart);
    ui->chart_layout->addWidget(chartview);
    connect(chartview,SIGNAL(move_value(QPointF)),this,SLOT(chart_move_value(QPointF)));
    data_start_row=0;
    line_color = QColor("#ff0000");
}

void etching_analysor::analysor(QString filepath)
{
    QFile *analysor_file = new QFile(filepath);
    analysor_file->open(QIODevice::ReadOnly | QIODevice::Text);
    data_start_row = 0;
    while(!analysor_file->atEnd())
    {
      dataline<<analysor_file->readLine();
    }
    QString data = dataline.at(4);
    QRegularExpression patten;
    patten.setPattern("waferId: .+?\\t");
    QRegularExpressionMatch match = patten.match(data);
    if(match.hasMatch()){
        QString match_str = match.captured(0);
        QString temp_data = match_str.split(":").at(1);
        temp_data=temp_data.replace("\\t"," ");
        ui->La_waferid->setText(temp_data);
    }
    patten.setPattern("Recipe: .+?\\t");
    match = patten.match(data);
    if(match.hasMatch()){
        QString match_str = match.captured(0);
        QString temp_data = match_str.split(":").at(1);
        temp_data=temp_data.replace("\\t"," ");
        ui->La_recipe->setText(temp_data);
    }
    patten.setPattern("lotID: .+?\\t");
    match = patten.match(data);
    if(match.hasMatch()){
        QString match_str = match.captured(0);
        QString temp_data = match_str.split(":").at(1);
        temp_data=temp_data.replace("\\t"," ");
        ui->La_LotID->setText(temp_data);
    }
    analysor_file->close();


    for(int i=0;i<dataline.size();i++){
         QString readline = dataline.at(i);
         if(readline.indexOf("HistoricalData:")>=0){
             data_start_row = i+1;
            break;
         }
    }
    data = dataline.at(data_start_row);
    patten.setPattern("\\S+");
    QRegularExpressionMatchIterator match_ite = patten.globalMatch(data);
    ui->cb_type->clear();
    ui->cb_type->addItem("");
    while(match_ite.hasNext()){
        ui->cb_type->addItem(match_ite.next().captured());
    }
    data_start_row++;
    QString data_type = dataline.at(data_start_row);

    QByteArray temp_data_type= data_type.toLocal8Bit();
    temp_data_type = temp_data_type.replace(0x09,'$');
    QString reslut_data = QString(temp_data_type);
    data_type_list = reslut_data.split('$');

}

etching_analysor::~etching_analysor()
{
    delete ui;
}

void etching_analysor::on_select_file_clicked()
{
    QString filepath = QFileDialog::getOpenFileName(this,tr("Open File"),qApp->applicationDirPath(),tr("log (*.log)"));
    analysor(filepath);
}

void etching_analysor::read_ready_log()
{
    qDebug()<<"read_ready !";
}

void etching_analysor::chart_move_value(QPointF value)
{
    ui->X_value->setText(QString("%1").arg(value.x(),0,'f',2));
    ui->Y_value->setText(QString("%1").arg(value.y(),0,'f',2));
}

void etching_analysor::on_cb_type_currentIndexChanged(int index)
{
    if(index !=0 ){
        QString data = data_type_list.at(index-1);
        QStringList data_list = data.split(",");
        ui->cb_type_2->clear();
        ui->cb_type_2->addItem(" ");
        ui->cb_type_2->addItem(data_list.at(1));
    }
}

void etching_analysor::on_cb_type_2_currentIndexChanged(int index)
{
    if(index != 0){
        disconnect(chartview,SIGNAL(move_value(QPointF)),this,SLOT(chart_move_value(QPointF)));
        ui->chart_layout->removeWidget(chartview);
        line->deleteLater();
        chart->deleteLater();
        chartview->deleteLater();


        line = new QLineSeries();
        line->setColor(QColor("#00aaff"));
        line->setName(ui->cb_type->currentText());
        for(int i=data_start_row+1;i<dataline.size();i++){
            QString data = dataline.at(i);
            QByteArray data_temp = data.toLocal8Bit();
            data_temp = data_temp.replace(0x09,'$');
            QString result_data = QString(data_temp);
            QStringList result_list = result_data.split('$');
            QString x_y_data = result_list.at(ui->cb_type->currentIndex()-1);
            QStringList result_data_list = x_y_data.split(',');
            //qDebug()<<"X = "<<result_data_list.at(0)<<"Y = "<<result_data_list.at(1);
            QString x_value_str = result_data_list.at(0);
            QString Y_value_str = result_data_list.at(1);
            int find_str_x =  x_value_str.indexOf("-");
            int find_str_y =  Y_value_str.indexOf("-");
            if(find_str_x < 0 && find_str_y<0){
                line->append(x_value_str.toDouble(),Y_value_str.toDouble());
            }
        }
        chart = new etching_chart_1(line);
        chart->axisX()->setTitleText("msec");
        chart->axisY()->setTitleText(ui->cb_type_2->currentText());
        chartview = new etching_chart_1_view(chart);
        connect(chartview,SIGNAL(move_value(QPointF)),this,SLOT(chart_move_value(QPointF)));
        ui->chart_layout->addWidget(chartview);
    }
}

void etching_analysor::on_zoomreset_btn_clicked()
{
    chart->zoomReset();
}

void etching_analysor::on_line_add_btn_clicked()
{
    QLineSeries *new_line =  new QLineSeries();
    new_line->setPointsVisible(true);
    new_line->setColor(line_color);
    new_line->setName(ui->cb_newline_name->currentText());
    QList<QPointF> list_point = line->points();
    for(int i=0;i<list_point.size();i++){
        if(i==0){
            qreal ydata = ui->newline_value->text().toDouble();
            qDebug()<<"int 0 = "<<ydata;
            new_line->append(list_point.at(i).x(),ydata);
        }else {
            qreal ydata = ui->newline_value->text().toDouble();
            qDebug()<<"int other = "<<ydata;
            new_line->append(list_point.at(i).x(),ydata);
        }
    }

    chart->addSeries(new_line);
    chart->createDefaultAxes();

}

void etching_analysor::on_select_color_btn_clicked()
{
    line_color = QColorDialog::getColor();
    ui->line_colorview->setStyleSheet(QString("background-color: %1;").arg(line_color.name()));
}
