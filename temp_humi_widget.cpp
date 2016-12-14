#include "temp_humi_widget.h"
#include "ui_temp_humi_widget.h"

temp_humi_widget::temp_humi_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::temp_humi_widget)
{
    ui->setupUi(this);
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

void temp_humi_widget::host_connect()
{
//    qDebug()<<"connect";
    temp_humi_time_out();

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


