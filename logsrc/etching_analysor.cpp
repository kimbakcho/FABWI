#include "etching_analysor.h"
#include "ui_etching_analysor.h"

etching_analysor::etching_analysor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::etching_analysor)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    QHeaderView *tempheader;
    tempheader = ui->chart_listview->horizontalHeader();

    int size_refrom = this->width()-((this->width()/4)*1);
    tempheader->resizeSection(1,size_refrom-20);
    size_refrom = this->width()-((this->width()/4)*3);
    tempheader->resizeSection(2,size_refrom-20);

    tempheader->resizeSection(0,25); //체크박스 사이즈 변경

    file_drag_count= 0;

    base_chartview_list.append(ui->logview_0);
    base_chartview_list.append(ui->logview_1);
    base_chartview_list.append(ui->logview_2);
    base_chartview_list.append(ui->logview_3);
    base_chartview_list.append(ui->logview_4);
    base_chartview_list.append(ui->logview_5);
    base_chartview_list.append(ui->logview_6);
    base_chartview_list.append(ui->logview_7);
    base_chartview_list.append(ui->logview_8);
}

void etching_analysor::analysor(etching_analysor_item *item)
{
    ui->logview_0->append_file_item(item);
    ui->logview_0->set_item_list_box(item->item_list);
    ui->logview_1->append_file_item(item);
    ui->logview_1->set_item_list_box(item->item_list);
    ui->logview_2->append_file_item(item);
    ui->logview_2->set_item_list_box(item->item_list);
    ui->logview_3->append_file_item(item);
    ui->logview_3->set_item_list_box(item->item_list);
    ui->logview_4->append_file_item(item);
    ui->logview_4->set_item_list_box(item->item_list);
    ui->logview_5->append_file_item(item);
    ui->logview_5->set_item_list_box(item->item_list);
    ui->logview_6->append_file_item(item);
    ui->logview_6->set_item_list_box(item->item_list);
    ui->logview_7->append_file_item(item);
    ui->logview_7->set_item_list_box(item->item_list);
    ui->logview_8->append_file_item(item);
    ui->logview_8->set_item_list_box(item->item_list);

    if(file_drag_count==1){
        ui->logview_0->set_item_index(0);
        ui->logview_1->set_item_index(1);
        ui->logview_2->set_item_index(2);
        ui->logview_3->set_item_index(3);
        ui->logview_4->set_item_index(4);
        ui->logview_5->set_item_index(5);
        ui->logview_6->set_item_index(6);
        ui->logview_7->set_item_index(7);
        ui->logview_8->set_item_index(8);
    }else {
        ui->logview_0->signal_cb_item_current("auto");
        ui->logview_1->signal_cb_item_current("auto");
        ui->logview_2->signal_cb_item_current("auto");
        ui->logview_3->signal_cb_item_current("auto");
        ui->logview_4->signal_cb_item_current("auto");
        ui->logview_5->signal_cb_item_current("auto");
        ui->logview_6->signal_cb_item_current("auto");
        ui->logview_7->signal_cb_item_current("auto");
        ui->logview_8->signal_cb_item_current("auto");
    }
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
    QList<QUrl> url_data = event->mimeData()->urls();
    for(int i=0;i<url_data.size();i++){
        QString file_path = url_data.at(i).toString(QUrl::PreferLocalFile);
        etching_analysor_item *item = new etching_analysor_item();
        logfile.append(item);
        connect(item,SIGNAL(change_linevisible(bool,QColor,QString)),this,SLOT(change_line_show(bool,QColor,QString)));
        item->setFile_path(file_path);
        item->set_check_box_text(url_data.at(i).fileName());
        int rowcount = ui->chart_listview->rowCount();
        ui->chart_listview->insertRow(rowcount);
        ui->chart_listview->setCellWidget(rowcount,0,item->getCheck_box());
        ui->chart_listview->setCellWidget(rowcount,1,item->getLA_information());
        ui->chart_listview->setCellWidget(rowcount,2,item->getLA_file_path());
        file_drag_count++;
        analysor(item);
    }

}

void etching_analysor::resizeEvent(QResizeEvent *event)
{
    QHeaderView *tempheader;
    tempheader = ui->chart_listview->horizontalHeader();

    int size_refrom = this->width()-((this->width()/4)*1);
    tempheader->resizeSection(1,size_refrom-20);
    size_refrom = this->width()-((this->width()/4)*3);
    tempheader->resizeSection(2,size_refrom-20);
    ui->total_chart_layout->setColumnMinimumWidth(0,(this->width()/3)-20);
    ui->total_chart_layout->setColumnMinimumWidth(1,(this->width()/3)-20);
    ui->total_chart_layout->setColumnMinimumWidth(2,(this->width()/3)-20);

}

void etching_analysor::closeEvent(QCloseEvent *event)
{
    for(int i=0;i<base_chartview_list.count();i++){
        base_chartview_list.at(i)->mainchart->deleteLater();
        base_chartview_list.at(i)->chartview->deleteLater();
        base_chartview_list.at(i)->deleteLater();
    }
//    for(int i=0;i<logfile.count();i++){
//        for(int j=0;j<logfile.at(i)->lines.count();j++){
//            logfile.at(i)->lines.at(j)->line.deleteLater();
//        }
//    }

    this->deleteLater();

}


void etching_analysor::on_chart_listview_cellDoubleClicked(int row, int column)
{
    QLabel *name = (QLabel *)ui->chart_listview->cellWidget(row,1);
    QString str_value = name->text();
    QStringList temp1 = str_value.split('\t');
    QString temp2 = temp1.at(0);
    QStringList temp3 = temp2.split("=");
    QString waferid = temp3.at(1);
    waferid = waferid + '\t';
    QString color_name = name->styleSheet().split("#").at(1);
    QColor set_color(QString("#%1").arg(color_name));


    QColor choice_color = QColorDialog::getColor();
    logfile.at(row)->lineColor = choice_color;
    int count = logfile.at(row)->lines.count();
    for(int i=0;i<count;i++){
        logfile.at(row)->lines.at(i)->line.setColor(choice_color);
    }
    for(int j=0;j<base_chartview_list.size();j++){
        int count = base_chartview_list.at(j)->mainchart->series().size();
        for(int i=0;i<count;i++){
            QLineSeries *templine  = (QLineSeries *)base_chartview_list.at(j)->mainchart->series().at(i);
            QString tempcolorname = templine->color().name();
            if((set_color.name() == tempcolorname)&&(templine->name() == waferid)){
                 name->setStyleSheet(QString("background-color : %1").arg(choice_color.name()));
                 templine->setColor(choice_color);
            }
        }
    }
}

void etching_analysor::change_line_show(bool ischeck, QColor linecolor, QString waferid)
{
    for(int j=0;j<base_chartview_list.size();j++){
        int count = base_chartview_list.at(j)->mainchart->series().size();
        for(int i=0;i<count;i++){
            QLineSeries *templine  = (QLineSeries *)base_chartview_list.at(j)->mainchart->series().at(i);
            QString tempcolorname = templine->color().name();
            qDebug()<<waferid;
            qDebug()<<"tempcolorname = "<<tempcolorname << "linecolor.name = "<<linecolor.name();
            if((linecolor.name() == tempcolorname)&&(templine->name() == waferid)){
                 templine->setVisible(ischeck);
            }
        }
    }
}


