#include "eis_list_view.h"
#include "ui_eis_list_view.h"
#include "ui_eis_listview_item.h"

Eis_list_view::Eis_list_view(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Eis_list_view)
{
    ui->setupUi(this);
}
Eis_list_view::Eis_list_view(QStringList check_list,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Eis_list_view)
{
    ui->setupUi(this);
    for(int i=0;i<check_list.count();i++){
        QString src_value = check_list.at(i);
        QString doc_number = src_value.split("/").at(0);
        QString doc_write_time = src_value.split("/").at(1);
        EIS_listview_item *item = new EIS_listview_item(src_value,this);
        ui->item_listview->insertRow(i);
        ui->item_listview->setRowHeight(i,800);
        ui->item_listview->setCellWidget(i,0,item);
        item_list.append(item);
    }
}

void Eis_list_view::resizeEvent(QResizeEvent *event)
{
    int setHight = ui->item_listview->height();
    for(int i=0;i<ui->item_listview->rowCount();i++){
        ui->item_listview->setRowHeight(i,setHight-50);
    }
}

void Eis_list_view::modify_button_show(bool choice)
{
    for(int i=0;i<item_list.count();i++){
        item_list.at(i)->ui->modify_button->setVisible(choice);
    }
}

Eis_list_view::~Eis_list_view()
{
    delete ui;
}
