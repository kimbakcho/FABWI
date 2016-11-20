#include "etching_base_chart_view.h"
#include "ui_etching_base_chart_view.h"
#include "ui_etching_chart_bigview.h"
Etching_base_chart_view::Etching_base_chart_view(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Etching_base_chart_view)
{
    ui->setupUi(this);
    line_color = QColor("#ff0000");
    mainchart = new etching_chart_1();
    chartview = new etching_chart_1_view(mainchart);
    ui->chart_layout->addWidget(chartview);
    connect(chartview,SIGNAL(move_value(QPointF)),this,SLOT(chart_move_value(QPointF)));

}

void Etching_base_chart_view::set_item_list_box(QStringList value)
{
    bool contain = false;
    for(int i=0;i<value.size();i++){
          contain = false;
          int cb_count = ui->cb_item->count();
          for(int j=0;j<cb_count;j++){
             if(value.at(i) == ui->cb_item->itemText(j)){
                 contain = true;
             }
          }
          if(!contain){
              ui->cb_item->addItem(value.at(i));
          }
    }
}

void Etching_base_chart_view::set_item_index(int index)
{
    ui->cb_item->setCurrentIndex(index);
}

void Etching_base_chart_view::set_item_name(QString str)
{
    ui->cb_item->setCurrentText(str);
}

void Etching_base_chart_view::append_file_item(etching_analysor_item *file_item)
{
    file_item_list.append(file_item);
}

void Etching_base_chart_view::signal_cb_item_current(const QString &text)
{
    on_cb_item_currentIndexChanged(text);
}


QColor Etching_base_chart_view::getLine_color() const
{
    return line_color;
}

void Etching_base_chart_view::setLine_color(const QColor &value)
{
    line_color = value;
}

Etching_base_chart_view::~Etching_base_chart_view()
{
    delete ui;
}

//void Etching_base_chart_view::on_cb_type_currentIndexChanged(int index)
//{

    //        QVector<QLineSeries *> *line_list =  mainchart->linedata_list;
    //        for(int i=0;i<line_list->count();i++){
    //            QLineSeries *delete_line = line_list->at(i);
    //            mainchart->removeSeries(delete_line);
    //            delete_line->deleteLater();
    //        }
    //        line_list->clear();
    //        main_line = new QLineSeries();


    //        line_list->append(main_line);
    //        main_line->setColor(QColor("#00aaff"));
    //        main_line->setName(ui->cb_item->currentText());
    //        for(int i=data_start_row+1;i<log_data.size();i++){
    //            QString data = log_data.at(i);
    //            QByteArray data_temp = data.toLocal8Bit();
    //            data_temp = data_temp.replace(0x09,'$');
    //            QString result_data = QString(data_temp);
    //            QStringList result_list = result_data.split('$');
    //            QString x_y_data = result_list.at(ui->cb_item->currentIndex()-1);
    //            QStringList result_data_list = x_y_data.split(',');
    //            QString X_value_str = result_data_list.at(0);
    //            QString Y_value_str = result_data_list.at(1);
    //            //qDebug()<<"X = "<<result_data_list.at(0)<<"Y = "<<result_data_list.at(1);
    //            int find_str_x =  X_value_str.indexOf("-");
    //            int find_str_y =  Y_value_str.indexOf("-");
    //            if(find_str_x < 0 && find_str_y<0){

    //                main_line->append(X_value_str.toDouble(),Y_value_str.toDouble());
    //            }
    //        }
    //        mainchart->addSeries(main_line);
    //        main_line->setName(ui->cb_item->currentText());
    //        main_line->setPointsVisible(true);

    //        mainchart->createDefaultAxes();
    //        mainchart->axisX()->setTitleText("msec");
    //        mainchart->axisY()->setTitleText(ui->cb_type->currentText());


//}

void Etching_base_chart_view::on_select_color_btn_clicked()
{
    line_color = QColorDialog::getColor();
    ui->line_colorview->setStyleSheet(QString("background-color: %1;").arg(line_color.name()));
}

void Etching_base_chart_view::on_zoomreset_btn_clicked()
{
    mainchart->zoomReset();
}

void Etching_base_chart_view::chart_move_value(QPointF value)
{
    ui->X_value->setText(QString("%1").arg(value.x(),0,'f',2));
    ui->Y_value->setText(QString("%1").arg(value.y(),0,'f',2));
}

void Etching_base_chart_view::on_view_linelabel_btn_clicked()
{
    if(ui->view_linelabel_btn->text() == tr("view")){
          ;
         for(int i=0;i<mainchart->series().count();i++){
             QLineSeries *line = (QLineSeries *)mainchart->series().at(i);
             line->setPointLabelsVisible(true);
         }
        ui->view_linelabel_btn->setText(tr("no view"));

    }else {
         for(int i=0;i<mainchart->series().count();i++){
             QLineSeries *line = (QLineSeries *)mainchart->series().at(i);
             line->setPointLabelsVisible(false);
         }
       ui->view_linelabel_btn->setText(tr("view"));
    }
}

void Etching_base_chart_view::on_bigview_btn_clicked()
{
    etching_chart_bigview *bigview = new etching_chart_bigview(this);

    bigview->show();
}

void Etching_base_chart_view::on_line_add_btn_clicked()
{
        QLineSeries *new_line =  new QLineSeries();
        mainchart->linedata_list->append(new_line);
        new_line->setPointsVisible(true);
        new_line->setColor(line_color);

        new_line->setName(ui->newline_spec->currentText());
        QLineSeries * mainline = (QLineSeries *) mainchart->series().at(0);
        QList<QPointF> list_point =mainline->points();
        for(int i=0;i<list_point.size();i++){
                qreal ydata = ui->newline_value->text().toDouble();
                new_line->append(list_point.at(i).x(),ydata);
        }
        mainchart->addSeries(new_line);
        mainchart->createDefaultAxes();
}

void Etching_base_chart_view::on_cb_item_currentIndexChanged(const QString &text)
{
    QString text_data = text;
    if(text_data=="auto"){
        text_data = ui->cb_item->currentText();
    }
    int chartseirs_count = mainchart->series().count();
    for(int i=0;i<chartseirs_count;i++){
        mainchart->removeSeries(mainchart->series().at(0));
    }
    for(int i=0;i<file_item_list.count();i++){
        int count = file_item_list.at(i)->lines.count();
        for(int j=0;j<count;j++){
            QString item_name = file_item_list.at(i)->lines.at(j)->item_name;
            if(text_data == item_name){
                QLineSeries *new_line = new QLineSeries();
                int line_data_count = file_item_list.at(i)->lines.at(j)->line.count();
                for(int k=0;k<line_data_count;k++){
                    new_line->append(file_item_list.at(i)->lines.at(j)->line.at(k).x(),
                                     file_item_list.at(i)->lines.at(j)->line.at(k).y());
                }
                new_line->setColor(file_item_list.at(i)->lines.at(j)->line.color());
                new_line->setName(file_item_list.at(i)->lines.at(j)->line.name());
                new_line->setPointsVisible(true);
                mainchart->addSeries(new_line);
                ui->LA_type_2->setText(file_item_list.at(i)->lines.at(j)->item_type);
                break;
            }
        }
    }
    mainchart->createDefaultAxes();
    mainchart->axisX()->setTitleText("mesc");
    mainchart->axisY()->setTitleText(ui->LA_type_2->text());
}
