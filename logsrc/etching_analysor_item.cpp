#include "etching_analysor_item.h"
#include "ui_etching_analysor_item.h"

etching_analysor_item::etching_analysor_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::etching_analysor_item)
{
    ui->setupUi(this);
    check_box = ui->checkBox;
    LA_file_name = ui->LA_file_name;
    LA_information = ui->LA_information;
    lineColor = QColor(rand()/250,rand()/250,rand()/250);

    data_start_row = 0;

}

void etching_analysor_item::set_check_box_text(QString str)
{
    ui->LA_file_name->setText(str);
}

QColor etching_analysor_item::getLineColor() const
{
    return lineColor;
}

void etching_analysor_item::setLineColor(const QColor &value)
{
    lineColor = value;
}

QLabel *etching_analysor_item::getLA_information() const
{
    return LA_information;
}

void etching_analysor_item::setLA_information(QLabel *value)
{
    LA_information = value;
}

QLabel *etching_analysor_item::getLA_file_path() const
{
    return LA_file_name;
}

void etching_analysor_item::setLA_file_path(QLabel *value)
{
    LA_file_name = value;
}

QCheckBox *etching_analysor_item::getCheck_box() const
{
    return check_box;
}

void etching_analysor_item::setCheck_box(QCheckBox *value)
{
    check_box = value;
}

QString etching_analysor_item::getFile_path() const
{
    return file_path;
}

void etching_analysor_item::setFile_path(const QString &value)
{

    LA_file_name->setText(value.split("/").last());
    file_path = value;

    QString recipe;
    QString lot_ID;
    QString start_end_time;
    QFile *analysor_file = new QFile(file_path);
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
        waferid = temp_data;
    }
    patten.setPattern("Recipe: .+?\\t");
    match = patten.match(data);
    if(match.hasMatch()){
        QString match_str = match.captured(0);
        QString temp_data = match_str.split(":").at(1);
        temp_data=temp_data.replace("\\t"," ");
        recipe = temp_data;
    }
    patten.setPattern("lotID: .+?\\t");
    match = patten.match(data);
    if(match.hasMatch()){
         QString match_str = match.captured(0);
         QString temp_data = match_str.split(":").at(1);
         temp_data=temp_data.replace("\\t"," ");
         lot_ID = temp_data;
    }
    analysor_file->close();
    start_end_time = dataline.at(10);
    LA_information->setText(QString("waferid=%1/Recipe = %2/lotID=%3/ %4").arg(waferid).arg(recipe).arg(lot_ID).arg(start_end_time));

    for(int i=0;i<dataline.size();i++){
         QString readline = dataline.at(i);
         if(readline.indexOf("HistoricalData:")>=0){
             data_start_row = i+1;
            break;
         }
    }
    QString data_str = dataline.at(data_start_row);
    patten.setPattern("\\S+");
    QRegularExpressionMatchIterator match_ite = patten.globalMatch(data_str);
    while(match_ite.hasNext()){
        QString captureed_data;
        captureed_data =match_ite.next().captured();
        for(int i=0;i<item_list.count();i++){
               if(item_list.at(i) == captureed_data){
                   captureed_data = captureed_data + "_2";
               }
        }
        item_list<<captureed_data;
        line_item *temp_item = new line_item();
        temp_item->item_name = captureed_data;
        lines.append(temp_item);
    }

    data_start_row++;

    QString data_type = dataline.at(data_start_row);

    QByteArray temp_data_type= data_type.toLocal8Bit();
    temp_data_type = temp_data_type.replace(0x09,'$');
    QString reslut_data = QString(temp_data_type);
    QStringList type_data_list = reslut_data.split('$');
    for(int i=0;i<type_data_list.count()-1;i++){
        QString temp_type = type_data_list.at(i);
        lines.at(i)->item_type = temp_type.split(',').at(1);
    }


    LA_information->setStyleSheet(QString("background-color : %1").arg(lineColor.name()));
    for(int i=0;i<lines.count();i++){
        for(int j=data_start_row+1;j<dataline.size();j++){
            QString data = dataline.at(j);
            QByteArray data_temp = data.toLocal8Bit();
            data_temp = data_temp.replace(0x09,'$');
            QString result_data = QString(data_temp);
            QStringList result_list = result_data.split('$');
            QString x_y_data = result_list.at(i);
            QStringList result_data_list = x_y_data.split(',');
            QString X_value_str = result_data_list.at(0);
            double x_value = X_value_str.toDouble() / 1000.0;
            QString Y_value_str = result_data_list.at(1);
            int find_str_x =  X_value_str.indexOf("-");
            int find_str_y =  Y_value_str.indexOf("-");
            if(find_str_x < 0 && find_str_y<0){
                lines.at(i)->line.append(x_value,Y_value_str.toDouble());
            }
          }
          lines.at(i)->line.setPointLabelsFormat("(@yPoint)");
          lines.at(i)->line.setName(waferid);
          lines.at(i)->line.setPointsVisible(true);
          lines.at(i)->line.setColor(lineColor);
    }
}

etching_analysor_item::~etching_analysor_item()
{
    delete ui;
}


void etching_analysor_item::on_checkBox_toggled(bool checked)
{
    for(int i=0;i<lines.count();i++){
        lines.at(i)->line.setVisible(checked);
    }
    emit change_linevisible(checked,lineColor,waferid);
}
