#include "eismain.h"
#include "ui_eismain.h"
#include <QDebug>

EISmain::EISmain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EISmain)
{
    ui->setupUi(this);
    QHeaderView *tempheader;
    tempheader = ui->tableWidget_2->horizontalHeader();
    connect(tempheader,SIGNAL(sectionClicked(int)),this,SLOT(header_click(int)));

}

EISmain::~EISmain()
{
    delete ui;
}

void EISmain::on_tableWidget_2_clicked(const QModelIndex &index)
{
    qDebug()<<"test";
}

void EISmain::on_tableWidget_2_itemClicked(QTableWidgetItem *item)
{
    qDebug()<<"test";
}
void EISmain::header_click(int data){
    qDebug()<<"test";
}
