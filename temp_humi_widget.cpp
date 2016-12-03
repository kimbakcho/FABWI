#include "temp_humi_widget.h"
#include "ui_temp_humi_widget.h"

temp_humi_widget::temp_humi_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::temp_humi_widget)
{
    ui->setupUi(this);
    ui->set_time->setTime(QTime::currentTime());
    connect(&plantimer,SIGNAL(timeout()),this,SLOT(planloop()));
    mainmodel = new QStandardItemModel();

    ui->plan_list->setModel(mainmodel);

    configini_str =qApp->applicationDirPath()+"/temp_humiplan.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    int setting_size = settings.beginReadArray("time_plan");
    for(int i=0;i<setting_size;i++){
        settings.setArrayIndex(i);
        mainmodel->appendRow(new QStandardItem(settings.value("paln_time").toString()));
    }
    settings.endArray();
    settings.beginGroup("file_path");

    settings.endGroup();

    plantimer.setInterval(300);
    plantimer.start();
    socket.connectToHost("10.22.10.60",2004);

    connect(&socket,SIGNAL(connected()),this,SLOT(host_connect()));
    connect(&socket,SIGNAL(readyRead()),this,SLOT(read_ready()));

    connect(&temp_humi_timer,SIGNAL(timeout()),this,SLOT(temp_humi_time_out()));

    temp_humi_timer.setInterval(8000);
    temp_humi_timer.start();

}

temp_humi_widget::~temp_humi_widget()
{
    delete ui;
}


//void temp_humi_widget::on_testbtn_clicked()
//{
//    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
//    excel->setProperty("Visible", true);
//    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
//    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", ui->LE_filepath->text());
//    QAxObject* sheets = workbook->querySubObject("Worksheets(QString)", "Sheet1");
//    QAxObject* range = sheets->querySubObject("Range(QString)", "B3");
//    QAxObject* range1 = sheets->querySubObject("Range(QString)", "B4");
//    qDebug()<<range->dynamicCall("Value").toString();
//    range1->setProperty("Value","4");
//    workbook->dynamicCall("Close(bool)",true);
//    excel->dynamicCall("Quit()");
//}

void temp_humi_widget::planloop()
{
    ui->LA_currentime->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void temp_humi_widget::host_connect()
{
    qDebug()<<"connect";
    temp_humi_time_out();

}

void temp_humi_widget::on_Btn_time_add_clicked()
{
    mainmodel->appendRow(new QStandardItem(ui->set_time->text()));
    configini_str =qApp->applicationDirPath()+"/temp_humiplan.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginWriteArray("time_plan");
    for(int i=0;i<mainmodel->rowCount();i++){
        settings.setArrayIndex(i);
        settings.setValue("paln_time",mainmodel->index(i,0).data().toString());
    }
    settings.endArray();
}

void temp_humi_widget::model_change()
{

}

void temp_humi_widget::read_ready()
{
    QByteArray data = socket.readAll();
    QByteArray find_word;
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);
    find_word.append((char)0x00);

    for(int i=0;i<data.count();i++){
        join_data.append(data.at(i));
        if(join_data.count()==1 && data.at(i) != 0x4c){
            join_data.clear();
        }
        if(join_data.count()>=8){
             if(QString(join_data) == "LSIS-XGT"){
                if(join_data.indexOf(find_word)>=0){
                    datalist.append(join_data);
                    join_data.clear();
                }
             }
        }
    }
    for(int i=0;i<datalist.count();i++){
        QByteArray reslut_data = datalist.at(i);
        //애칭#1 data
        short temp_data = (unsigned char)reslut_data.at(35);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(34);
        short humi_data = (unsigned char)reslut_data.at(37);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(36);
        double temp_data_real = (double)temp_data/(double)10.0;
        double humi_data_real = (double)humi_data/(double)10.0;
        ui->etching_temp1->display(temp_data_real);
        ui->etching_humi1->display(humi_data_real);

        //노광#1 data
        temp_data = (unsigned char)reslut_data.at(39);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(38);
        humi_data = (unsigned char)reslut_data.at(41);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(40);
        temp_data_real = (double)temp_data/(double)10.0;
        humi_data_real = (double)humi_data/(double)10.0;
        ui->light1_temp->display(temp_data_real);
        ui->light1_humi->display(humi_data_real);

        //성막 온습도
        temp_data = (unsigned char)reslut_data.at(43);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(42);
        humi_data = (unsigned char)reslut_data.at(45);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(44);
        temp_data_real = (double)temp_data/(double)10.0;
        humi_data_real = (double)humi_data/(double)10.0;
        ui->taber_temp->display(temp_data_real);
        ui->taber_humi->display(humi_data_real);

        //노광#3 온습도
        temp_data = (unsigned char)reslut_data.at(47);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(46);
        humi_data = (unsigned char)reslut_data.at(49);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(48);
        temp_data_real = (double)temp_data/(double)10.0;
        humi_data_real = (double)humi_data/(double)10.0;
        ui->light3_temp->display(temp_data_real);
        ui->light3_humi->display(humi_data_real);

        //프로브1 온습도
        temp_data = (unsigned char)reslut_data.at(51);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(50);
        humi_data = (unsigned char)reslut_data.at(53);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(52);
        temp_data_real = (double)temp_data/(double)10.0;
        humi_data_real = (double)humi_data/(double)10.0;
        ui->probe1_temp->display(temp_data_real);
        ui->probe1_humi->display(humi_data_real);

        //에칭#2 온습도
        temp_data = (unsigned char)reslut_data.at(55);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(54);
        humi_data = (unsigned char)reslut_data.at(57);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(56);
        temp_data_real = (double)temp_data/(double)10.0;
        humi_data_real = (double)humi_data/(double)10.0;
        ui->etching2_temp->display(temp_data_real);
        ui->etching2_humi->display(humi_data_real);

        //노광 온습도
        temp_data = (unsigned char)reslut_data.at(59);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(58);
        humi_data = (unsigned char)reslut_data.at(61);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(60);
        temp_data_real = (double)temp_data/(double)10.0;
        humi_data_real = (double)humi_data/(double)10.0;
        ui->light2_temp->display(temp_data_real);
        ui->light2_humi->display(humi_data_real);


        //프로브2 온습도
        temp_data = (unsigned char)reslut_data.at(63);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(62);
        humi_data = (unsigned char)reslut_data.at(65);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(64);
        temp_data_real = (double)temp_data/(double)10.0;
        humi_data_real = (double)humi_data/(double)10.0;
        ui->probe2_temp->display(temp_data_real);
        ui->probe2_humi->display(humi_data_real);

        //약품 창고 온도
        temp_data = (unsigned char)reslut_data.at(67);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(66);
        temp_data_real = (double)temp_data/(double)10.0;
        ui->darg_temp->display(temp_data_real);

        //리프트오프룸 온습도
        temp_data = (unsigned char)reslut_data.at(75);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(74);
        humi_data = (unsigned char)reslut_data.at(77);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(76);
        temp_data_real = (double)temp_data/(double)10.0;
        humi_data_real = (double)humi_data/(double)10.0;
        ui->liftoffroom_temp->display(temp_data_real);
        ui->liftoffroom_humi->display(humi_data_real);

        //krf룸 온습도
        temp_data = (unsigned char)reslut_data.at(71);
        temp_data = temp_data<<8;
        temp_data = temp_data|(unsigned char)reslut_data.at(70);
        humi_data = (unsigned char)reslut_data.at(73);
        humi_data = humi_data<<8;
        humi_data = humi_data|(unsigned char)reslut_data.at(72);
        temp_data_real = (double)temp_data/(double)10.0;
        humi_data_real = (double)humi_data/(double)10.0;
        ui->KRFroom_temp->display(temp_data_real);
        ui->KRFroom_humi->display(humi_data_real);
    }
}

void temp_humi_widget::temp_humi_time_out()
{
    QByteArray write_data;
    char send_data[]={0x4c,0x53,0x49,0x53,0x2d,0x58,0x47,0x54,0x00,0x00,0x00,0x00,0xa0,
                 0x33,0x07,0x00,0x13,0x00,0x00,0x48,0x54,0x00,0x14,0x00,0x00,0x00,
                 0x01,0x00,0x07,0x00,0x25,0x44,0x42,0x32,0x30,0x30,0x30,0x64,0x00};
    write_data.append(send_data,sizeof(send_data));
    socket.write(send_data,sizeof(send_data));
    socket.flush();

}

void temp_humi_widget::on_Btn_time_del_clicked()
{
    mainmodel->removeRow(ui->plan_list->currentIndex().row());
    configini_str =qApp->applicationDirPath()+"/temp_humiplan.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginWriteArray("time_plan");
    for(int i=0;i<mainmodel->rowCount();i++){
        settings.setArrayIndex(i);
        settings.setValue("paln_time",mainmodel->index(i,0).data().toString());
    }
    settings.endArray();
}

void temp_humi_widget::on_sendmail_btn_clicked()
{

//    excel->setProperty("Visible", true);
//    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
//    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", ui->LE_filepath->text());
//    QAxObject* sheets = workbook->querySubObject("Worksheets(QString)", "Sheet1");
//    QAxObject* range = sheets->querySubObject("Range(QString)", "B3");
//    QAxObject* range1 = sheets->querySubObject("Range(QString)", "B4");
//    qDebug()<<range->dynamicCall("Value").toString();
//    range1->setProperty("Value","4");
//    workbook->dynamicCall("Close(bool)",true);
//    excel->dynamicCall("Quit()");
    QDir dir("\\\\fabsv.wisol.co.kr\\daily_temp_humi\\");
    QString today_dir = QDate::currentDate().toString("MM_dd");
    if(!dir.exists(today_dir)){
        dir.mkdir(today_dir);
    }
    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    excel->setProperty("Visible", true);
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)","\\\\fabsv.wisol.co.kr\\daily_temp_humi\\main.xlsx");
    QAxObject* sheets = workbook->querySubObject("Worksheets(QString)", "main");
    QAxObject* range = sheets->querySubObject("Range(QString)", "C2");
    range = range->querySubObject("End(Int)",(int)-4161);
    range = range->querySubObject("Offset(Int,Int)",(int)0,(int)1);
    range->setProperty("Value",QDate::currentDate().toString("yyyy/MM/dd"));
    range = range->querySubObject("Offset(Int,Int)",(int)1,(int)0);
    range->setProperty("Value",ui->light1_temp->value());
    range = range->querySubObject("Offset(Int,Int)",(int)1,(int)0);
    range->setProperty("Value",ui->light2_temp->value());
    range = range->querySubObject("Offset(Int,Int)",(int)1,(int)0);
    range->setProperty("Value",ui->light3_temp->value());

    range = sheets->querySubObject("Range(QString)", "C25");
    range = range->querySubObject("End(Int)",(int)-4161);
    range = range->querySubObject("Offset(Int,Int)",(int)0,(int)1);
    range->setProperty("Value",QDate::currentDate().toString("yyyy/MM/dd"));
    range = range->querySubObject("Offset(Int,Int)",(int)1,(int)0);
    range->setProperty("Value",ui->light1_humi->value());
    range = range->querySubObject("Offset(Int,Int)",(int)1,(int)0);
    range->setProperty("Value",ui->light2_humi->value());
    range = range->querySubObject("Offset(Int,Int)",(int)1,(int)0);
    range->setProperty("Value",ui->light3_humi->value());


    range = sheets->querySubObject("Range(QString)", "C46");
    range = range->querySubObject("End(Int)",(int)-4161);
    range = range->querySubObject("Offset(Int,Int)",(int)0,(int)1);
    range->setProperty("Value",QDate::currentDate().toString("yyyy/MM/dd"));
    range = range->querySubObject("Offset(Int,Int)",(int)1,(int)0);
    range->setProperty("Value",ui->light1_humi->value());
    range = range->querySubObject("Offset(Int,Int)",(int)1,(int)0);
    range->setProperty("Value",ui->light2_humi->value());
    range = range->querySubObject("Offset(Int,Int)",(int)1,(int)0);
    range->setProperty("Value",ui->light3_humi->value());

//    qDebug()<<range->dynamicCall("Value").toString();


//    workbook->dynamicCall("Close(bool)",true);
//    excel->dynamicCall("Quit()");


}
