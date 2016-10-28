#include "eismain.h"
#include "ui_eismain.h"

EISmain::EISmain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EISmain)
{
    ui->setupUi(this);

    QString configini_str = "server.ini";
    QString server_ip;
    QString db_port;

    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setverset");
    server_ip = settings.value("serverip").toString();
    db_port = settings.value("dbport").toString();
    settings.endGroup();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","EISDB");
    db.setHostName(server_ip);
    db.setPort(db_port.toInt());
    db.setUserName("fabuser");
    db.setPassword("wisolfab");
    db.setDatabaseName("FAB");


    if(!db.open()){
        int ret = QMessageBox::warning(this, tr("conntion false"),
                                       "server connection fail\n"
                                          ""+db.lastError().text(),
                                            QMessageBox::Close);
    }

    //docnumber을 +1 한값을 데이터베이스 업로드 시켜 문서가 서로 중첩되지 않게한다.
    QSqlQuery query(db);
    query.exec("select * from EIS_management");
    query.next();
    doc_number = query.value("document_number").toInt();
    update_doc_number = doc_number+1;
    QString str_query = QString("update EIS_management SET document_number = %1").arg(update_doc_number);
    query.exec(str_query);

    QHeaderView *tempheader;
    tempheader = ui->tableWidget_2->horizontalHeader();
    connect(tempheader,SIGNAL(sectionClicked(int)),this,SLOT(header_click(int)));
    BTextEdit *testedit = new BTextEdit(&doc_number,this);
    ui->layout_phenomenon->addWidget(testedit,0,0);

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
