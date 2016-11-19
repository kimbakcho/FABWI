#include "etching_analysor.h"
#include "ui_etching_analysor.h"

etching_analysor::etching_analysor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::etching_analysor)
{
    ui->setupUi(this);
    setAcceptDrops(true);
}

void etching_analysor::analysor(QString filepath)
{
    QFile *analysor_file = new QFile(filepath);
    analysor_file->open(QIODevice::ReadOnly | QIODevice::Text);
    data_start_row = 0;
    dataline.clear();
    data_type_list.clear();
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

    ui->LA_start_end_time->setText(dataline.at(10));


    ui->logview_0->setLog_data(dataline);
    ui->logview_1->setLog_data(dataline);
    ui->logview_2->setLog_data(dataline);
    ui->logview_3->setLog_data(dataline);
    ui->logview_4->setLog_data(dataline);
    ui->logview_5->setLog_data(dataline);
    ui->logview_6->setLog_data(dataline);
    ui->logview_7->setLog_data(dataline);
    ui->logview_8->setLog_data(dataline);

    ui->logview_0->set_item_name("processTime");
    ui->logview_0->set_type_index(1);
    ui->logview_1->set_item_name("pressure");
    ui->logview_1->set_type_index(1);
    ui->logview_2->set_item_name("RF1DCBiasSensor");
    ui->logview_2->set_type_index(1);
    ui->logview_3->set_item_name("RF1ForwardPowerSensor");
    ui->logview_3->set_type_index(1);
    ui->logview_4->set_item_name("RF1ReflectedPowerSens");
    ui->logview_4->set_type_index(1);
    ui->logview_5->set_item_name("Temperature1Temperatu");
    ui->logview_5->set_type_index(1);
    ui->logview_6->set_item_name("Temperature2Temperatu");
    ui->logview_6->set_type_index(1);
    ui->logview_7->set_item_name("ThrottleValveControll");
    ui->logview_7->set_type_index(1);



}

etching_analysor::~etching_analysor()
{
    delete ui;
}

//void etching_analysor::on_select_file_clicked()
//{
//    QString filepath = QFileDialog::getOpenFileName(this,tr("Open File"),qApp->applicationDirPath(),tr("log (*.log)"));
//    analysor(filepath);
//}

void etching_analysor::dragEnterEvent(QDragEnterEvent *event)
{
     event->acceptProposedAction();
}

void etching_analysor::dropEvent(QDropEvent *event)
{

    QString filepath = event->mimeData()->text();
    if(filepath.indexOf("file:")>=0){
         analysor(filepath.split("file:").at(1));
    }
}

void etching_analysor::resizeEvent(QResizeEvent *event)
{
    ui->total_chart_layout->setColumnMinimumWidth(0,(this->width()/3)-20);
    ui->total_chart_layout->setColumnMinimumWidth(1,(this->width()/3)-20);
    ui->total_chart_layout->setColumnMinimumWidth(2,(this->width()/3)-20);
}

