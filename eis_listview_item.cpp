#include "eis_listview_item.h"
#include "ui_eis_listview_item.h"

EIS_listview_item::EIS_listview_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EIS_listview_item)
{
    ui->setupUi(this);

}

EIS_listview_item::EIS_listview_item(QString doc_data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EIS_listview_item)
{
    ui->setupUi(this);
    progressdialog = 0;
    QString doc_number_str = doc_data.split("/").at(0);
    QString doc_write_time = doc_data.split("/").at(1);
    doc_number =doc_number_str.toInt();
    QString configini_str = "server.ini";

    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("setverset");
    server_ip = settings.value("serverip").toString();
    db_port = settings.value("dbport").toString();
    ftp_port = settings.value("ftpport").toString();
    settings.endGroup();

    ftp = new QFtp(this);

    connect(ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));
    connect(ftp,SIGNAL(rawCommandReply(int,QString)),
            this,SLOT(ftp_rawCommandReply(int,QString)));

    connect(ftp,SIGNAL(listInfo(QUrlInfo)),
            this,SLOT(ftp_listInfo(QUrlInfo)));
    connect(ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
            this, SLOT(updateDataTransferProgress(qint64,qint64)));


    progressdialog = new QProgressDialog(this);

    if(!db.contains("EISDB")){
       db = QSqlDatabase::addDatabase("QMYSQL","EISDB");
       db.setHostName(server_ip);
       db.setPort(db_port.toInt());
       db.setUserName("fabuser");
       db.setPassword("wisolfab");
       db.setDatabaseName("FAB");
    }else {
       db = QSqlDatabase::database("EISDB");
    }

    if(!db.isOpen()){
        if(!db.open()){
             int ret = QMessageBox::warning(this, tr("conntion false"),
                                               "server connection fail\n"
                                                  ""+db.lastError().text(),
                                                    QMessageBox::Close);
        }
    }

    QSqlQuery query_base_data(db);
    QString query_txt = QString("select * from EIS_document where idx = \'%1\' AND write_time = \'%2\'")
            .arg(doc_number_str).arg(doc_write_time);
    query_base_data.exec(query_txt);
    query_base_data.next();

    QString makedir_txt = qApp->applicationDirPath()+"/temp/EIS/img";

    QDir makedir(makedir_txt);
    if(!makedir.exists(doc_number_str)){
       makedir.mkdir(doc_number_str);
    }

    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding("EIS"),"1234");
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand(QString("CWD /home/EIS/img/%1").arg(doc_number));
    loop.exec();
    makedir_txt = qApp->applicationDirPath()+"/temp/EIS/img/"+QString("%1").arg(doc_number);
    img_download_file = query_base_data.value("downloadimg").toString();
    QStringList img_file_list = query_base_data.value("downloadimg").toString().split("/////");
    for(int i=0;i<img_file_list.count()-1;i++){
        QString file_name = img_file_list.at(i);
        QString des_file_path = makedir_txt+ file_name;
        QFile des_file(des_file_path);
        des_file.open(QFile::ReadWrite);
        ftp->get(file_name,&des_file);
        progressdialog->exec();
        des_file.close();
    }

    QSqlQuery query(db);
    query.exec("select team from People_information GROUP by team;");
    ui->select_team->addItem("");
    disconnect(ui->select_team,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(on_select_team_currentIndexChanged(QString)));
    disconnect(ui->select_process,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(on_select_process_currentIndexChanged(QString)));
    disconnect(ui->select_facilities,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(on_select_facilities_currentIndexChanged(QString)));

    while(query.next()){
        ui->select_team->addItem(query.value("team").toString());
    }
    ui->select_team->setCurrentText(query_base_data.value("team").toString());

    query_txt = QString("select factory_process from People_information where team = '%1' group by factory_process")
            .arg(query_base_data.value("team").toString());
    query.exec(query_txt);
    while(query.next()){
        ui->select_process->addItem(query.value("factory_process").toString());
    }
    ui->select_process->setCurrentText(query_base_data.value("factory_process").toString());

    query_txt = QString("select facilities from People_information where factory_process = '%1' group by facilities")
            .arg(query_base_data.value("factory_process").toString());
    query.exec(query_txt);
    while(query.next()){
        ui->select_facilities->addItem(query.value("facilities").toString());
    }
    ui->select_facilities->setCurrentText(query_base_data.value("facilities_name").toString());

    query_txt = QString("select name from People_information where facilities = '%1'")
            .arg(query_base_data.value("facilities_name").toString());
    query.exec(query_txt);
    while(query.next()){
        ui->select_name->addItem(query.value("name").toString());
    }
    ui->select_name->setCurrentText(query_base_data.value("witer_name").toString());

    connect(ui->select_team,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(on_select_team_currentIndexChanged(QString)));
    connect(ui->select_process,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(on_select_process_currentIndexChanged(QString)));
    connect(ui->select_facilities,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(on_select_facilities_currentIndexChanged(QString)));

    phenomenon_edit = new BTextEdit(&doc_number,this);
    ui->layout_phenomenon->addWidget(phenomenon_edit,0,0);
    cause_edit = new BTextEdit(&doc_number,this);
    ui->layout_cause->addWidget(cause_edit,0,0);
    current_action_edit = new BTextEdit(&doc_number,this);
    ui->layout_current_action->addWidget(current_action_edit,0,0);
    current_lot_action_edit = new BTextEdit(&doc_number,this);
    ui->layout_current_lot_action->addWidget(current_lot_action_edit,0,0);
    change_master_sheet_edit = new BTextEdit(&doc_number,this);
    ui->layout_change_master_sheet->addWidget(change_master_sheet_edit,0,0);
    next_shift_edit = new BTextEdit(&doc_number,this);
    ui->layout_next_shift->addWidget(next_shift_edit,0,0);
    part_change_edit = new BTextEdit(&doc_number,this);
    ui->layout_part_change->addWidget(part_change_edit,0,0);
    after_lot_monitering_edit = new BTextEdit(&doc_number,this);
    ui->layout_after_lot_monitering->addWidget(after_lot_monitering_edit,0,0);
    attach_list_model = new QStandardItemModel();
    ui->attach_listview->setModel(attach_list_model);

    QStringList attach_file_list = query_base_data.value("attach_file_list").toString().split("/////");
    for(int i=0;i<attach_file_list.count()-1;i++){
        attach_list_model->insertRow(i,new QStandardItem(attach_file_list.at(i)));
    }

    QString font_family = ui->font_type->fontInfo().family();
    int font_size = ui->fontsize->value();
    mainfont.setFamily(font_family);
    mainfont.setPointSize(font_size);

    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    phenomenon_edit->setCurrentCharFormat(charfotmet);
    cause_edit->setCurrentCharFormat(charfotmet);
    current_action_edit->setCurrentCharFormat(charfotmet);
    current_lot_action_edit->setCurrentCharFormat(charfotmet);
    change_master_sheet_edit->setCurrentCharFormat(charfotmet);
    next_shift_edit->setCurrentCharFormat(charfotmet);
    part_change_edit->setCurrentCharFormat(charfotmet);
    after_lot_monitering_edit->setCurrentCharFormat(charfotmet);


    ui->current_time->setText(query_base_data.value("write_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    ui->document_name->setText(query_base_data.value("document_name").toString());

    if(query_base_data.value("time_part").toInt()==0){
        ui->select_part_time->setCurrentIndex(0);
    }else if(query_base_data.value("time_part").toInt()==1){
        ui->select_part_time->setCurrentIndex(1);
    }

    if(query_base_data.value("change_have").toInt()==1){
        ui->radio_btn_change_exist->setChecked(true);
    }else if(query_base_data.value("change_have").toInt()==0){
        ui->radio_btn_change_none->setChecked(true);
    }


    phenomenon_edit->setHtml(query_base_data.value("phenomenon").toString());
    cause_edit->setHtml(query_base_data.value("cause").toString());
    current_action_edit->setHtml(query_base_data.value("current_action").toString());
    current_lot_action_edit->setHtml(query_base_data.value("Lot_action_statue").toString());
    change_master_sheet_edit->setHtml(query_base_data.value("change_master_sheet").toString());
    next_shift_edit->setHtml(query_base_data.value("next_shift_link").toString());
    part_change_edit->setHtml(query_base_data.value("part_change_statue").toString());
    after_lot_monitering_edit->setHtml(query_base_data.value("after_LOT_monitering").toString());


}

EIS_listview_item::~EIS_listview_item()
{
    delete ui;
}

void EIS_listview_item::on_select_team_currentIndexChanged(const QString &arg1)
{
    ui->select_process->clear();;
    ui->select_process->addItem("");
    QSqlQuery query(db);
    QString query_txt = QString("select factory_process from People_information where team = '%1' group by factory_process").arg(arg1);
    query.exec(query_txt);
    while(query.next()){
        ui->select_process->addItem(query.value("factory_process").toString());
    }
}

void EIS_listview_item::on_select_process_currentIndexChanged(const QString &arg1)
{
    ui->select_facilities->clear();;
    ui->select_facilities->addItem("");
    QSqlQuery query(db);
    QString query_txt = QString("select facilities from People_information where factory_process = '%1' group by facilities").arg(arg1);
    query.exec(query_txt);
    while(query.next()){
        ui->select_facilities->addItem(query.value("facilities").toString());
    }
}

void EIS_listview_item::on_select_facilities_currentIndexChanged(const QString &arg1)
{
    ui->select_name->clear();;
    ui->select_name->addItem("");
    QSqlQuery query(db);
    QString query_txt = QString("select name from People_information where facilities = '%1'").arg(arg1);
    query.exec(query_txt);
    while(query.next()){
        ui->select_name->addItem(query.value("name").toString());
    }
}


void EIS_listview_item::on_font_type_currentTextChanged(const QString &arg1)
{
    mainfont.setFamily(arg1);
    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    phenomenon_edit->setCurrentCharFormat(charfotmet);
    cause_edit->setCurrentCharFormat(charfotmet);
    current_action_edit->setCurrentCharFormat(charfotmet);
    current_lot_action_edit->setCurrentCharFormat(charfotmet);
    change_master_sheet_edit->setCurrentCharFormat(charfotmet);
    next_shift_edit->setCurrentCharFormat(charfotmet);
    part_change_edit->setCurrentCharFormat(charfotmet);
    after_lot_monitering_edit->setCurrentCharFormat(charfotmet);
}

void EIS_listview_item::on_fontsize_editingFinished()
{
    mainfont.setPointSize(ui->fontsize->value());
    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    phenomenon_edit->setCurrentCharFormat(charfotmet);
    cause_edit->setCurrentCharFormat(charfotmet);
    current_action_edit->setCurrentCharFormat(charfotmet);
    current_lot_action_edit->setCurrentCharFormat(charfotmet);
    change_master_sheet_edit->setCurrentCharFormat(charfotmet);
    next_shift_edit->setCurrentCharFormat(charfotmet);
    part_change_edit->setCurrentCharFormat(charfotmet);
    after_lot_monitering_edit->setCurrentCharFormat(charfotmet);
}
void EIS_listview_item::ftpCommandFinished(int commandId, bool error)
{

    loop.exit();
}

void EIS_listview_item::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    if(totalBytes==0){
        return;
    }
    progressdialog->setMaximum(totalBytes);
    progressdialog->setValue(readBytes);
}

void EIS_listview_item::ftp_listInfo(QUrlInfo urlInfo)
{
    qDebug()<<urlInfo.name();
}

void EIS_listview_item::ftp_rawCommandReply(int a, QString data)
{
       //qDebug()<<"ftp data = "<<data;
}

void EIS_listview_item::on_attach_listview_doubleClicked(const QModelIndex &index)
{
    QFileDialog file_dialog;
    QString source_file = index.data().toString();
    QString save_filepath = file_dialog.getSaveFileName(this,tr("Save File"),index.data().toString());
    qDebug()<<"save_filepath = "<<save_filepath;
    if(save_filepath==""){
        return;
    }
    QFile *save_file = new QFile(save_filepath);
    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding("EIS"),"1234");
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand(QString("CWD /home/EIS/attach/%1").arg(doc_number));
    loop.exec();
    save_file->open(QFile::ReadWrite);
    ftp->get(source_file,save_file);
    progressdialog->setLabelText("download");
    progressdialog->exec();
    save_file->close();
}
void EIS_listview_item::on_total_view_phenomenon_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->label_phenomenon->text(),phenomenon_edit,ui->layout_phenomenon);
    big_view->show();
}


void EIS_listview_item::on_total_view_cause_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->label_cause->text(),cause_edit,ui->layout_cause);
    big_view->show();
}

void EIS_listview_item::on_total_view_current_action_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->label_current_action->text(),current_action_edit,ui->layout_current_action);
    big_view->show();
}

void EIS_listview_item::on_total_view_current_lot_action_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->label_current_lot_action->text(),current_lot_action_edit,ui->layout_current_lot_action);
    big_view->show();
}

void EIS_listview_item::on_total_view_change_master_sheet_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->label_change_master_sheet->text(),change_master_sheet_edit,ui->layout_change_master_sheet);
    big_view->show();
}

void EIS_listview_item::on_total_view_next_shift_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->label_next_shift->text(),next_shift_edit,ui->layout_next_shift);
    big_view->show();
}

void EIS_listview_item::on_total_view_part_change_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->label_part_change->text(),part_change_edit,ui->layout_part_change);
    big_view->show();
}

void EIS_listview_item::on_total_view_after_lot_monitering_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->label_after_lot_monitering->text(),after_lot_monitering_edit,ui->layout_after_lot_monitering);
    big_view->show();
}

void EIS_listview_item::on_modify_button_clicked()
{
    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding("EIS"),"1234");
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand("CWD /home/EIS/img");
    loop.exec();
    ftp->rawCommand(QString("MKD %1").arg(doc_number));
    loop.exec();
    ftp->rawCommand(QString("CWD /home/EIS/img/%1").arg(doc_number));
    loop.exec();

    QString makedir_txt = qApp->applicationDirPath()+"/temp/EIS/img/"+QString("%1").arg(doc_number);
    QDir doc_dir(makedir_txt);
    QStringList filelist =  doc_dir.entryList(QDir::Files);
    for(int i=0;i<filelist.count();i++){
        if(progressdialog == 0){
            progressdialog = new QProgressDialog(this);
        }
        //qDebug()<<"filelist"<<filelist.at(i);
        QString des_file = makedir_txt+"/"+filelist.at(i);
        QFile *file = new QFile(des_file);
        ftp->put(file,filelist.at(i),QFtp::Binary);
        QString part = QString("%1/%2").arg(i).arg(filelist.count());
        progressdialog->setLabelText(part);
        progressdialog->exec();
    }

    QString now_datetime =QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    ui->current_time->setText(now_datetime);
    QString change_have;
    if(ui->radio_btn_change_exist->isChecked()){
        change_have = QString("%1").arg(1);
    }else {
        change_have = QString("%1").arg(0);
    }
    QSqlQuery query(db);
    QString Time_part = '1';


    if(ui->select_part_time->currentIndex()==0){
        Time_part = '0';
    }else if (ui->select_part_time->currentIndex()==1) {
        Time_part = '1';
    }
    QString document_name = ui->document_name->text();
    QString team_name = ui->select_team->currentText();
    QString process = ui->select_process->currentText();
    QString write_name = ui->select_name->currentText();
    QString facilities_name = ui->select_facilities->currentText();
    QString phenomenon = phenomenon_edit->tosqlhtml();
    QString cause = cause_edit->tosqlhtml();
    QString current_action = current_action_edit->tosqlhtml();
    QString current_lot_action = current_lot_action_edit->tosqlhtml();
    QString change_master_sheet = change_master_sheet_edit->tosqlhtml();
    QString after_lot_monitering = after_lot_monitering_edit->tosqlhtml();
    QString next_shift = next_shift_edit->tosqlhtml();
    QString part_change_statue = part_change_edit->tosqlhtml();
    QString attach_file_list;
    QStringList total_img_list;
    QString total_img_txt;

//    phenomenon_edit->setCurrentCharFormat(charfotmet);
//    cause_edit->setCurrentCharFormat(charfotmet);
//    current_action_edit->setCurrentCharFormat(charfotmet);
//    current_lot_action_edit->setCurrentCharFormat(charfotmet);
//    change_master_sheet_edit->setCurrentCharFormat(charfotmet);
//    next_shift_edit->setCurrentCharFormat(charfotmet);
//    part_change_edit->setCurrentCharFormat(charfotmet);
//    after_lot_monitering_edit->setCurrentCharFormat(charfotmet);

    for(int i=0;i<phenomenon_edit->image_list.count();i++){
        QString img_path = phenomenon_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }

    for(int i=0;i<cause_edit->image_list.count();i++){
        QString img_path = cause_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }

    for(int i=0;i<current_action_edit->image_list.count();i++){
        QString img_path = current_action_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }

    for(int i=0;i<current_lot_action_edit->image_list.count();i++){
        QString img_path = current_lot_action_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }

    for(int i=0;i<change_master_sheet_edit->image_list.count();i++){
        QString img_path = change_master_sheet_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }

    for(int i=0;i<next_shift_edit->image_list.count();i++){
        QString img_path = next_shift_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }

    for(int i=0;i<part_change_edit->image_list.count();i++){
        QString img_path = part_change_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }

    for(int i=0;i<after_lot_monitering_edit->image_list.count();i++){
        QString img_path = after_lot_monitering_edit->image_list.at(i);
        total_img_list.append(img_path.split("/").last());
    }
    for(int i=0;i<total_img_list.count();i++){
        QString img_file = total_img_list.at(i);
        total_img_txt = total_img_txt+img_file+"/////";
    }
    img_download_file = img_download_file + total_img_txt;
    for(int i=0;i<attach_list_model->rowCount();i++){
        QString item = attach_list_model->item(i)->text();
        attach_file_list = attach_file_list + item + "/////";
    }

    QString insert_query = QString("INSERT INTO EIS_document ("
                                   "`idx`,"
                                   "`team`,"
                                   "`write_time`,"
                                   "`factory_process`,"
                                   "`facilities_name`,"
                                   "`witer_name`,"
                                   "`change_have`,"
                                   "`time_part`,"
                                   "`document_name`,"
                                   "`phenomenon`,"
                                   "`cause`,"
                                   "`current_action`,"
                                   "`Lot_action_statue`,"
                                   "`change_master_sheet`,"
                                   "`next_shift_link`,"
                                   "`part_change_statue`,"
                                   "`after_LOT_monitering`,"
                                   "`downloadimg`,"
                                   "`attach_file_list`,"
                                   "`complete`"
                                   ") "
                                   "VALUES ("
                                   "'"+QString("%1").arg(doc_number)+"',"
                                   "'"+team_name+"',"
                                   "'"+now_datetime+"',"
                                   "'"+process+"',"
                                   "'"+facilities_name+"',"
                                   "'"+write_name+"',"
                                   "'"+change_have+"',"
                                   "'"+Time_part+"',"
                                   "'"+document_name+"',"
                                   "'"+phenomenon+"',"
                                   "'"+cause+"',"
                                   "'"+current_action+"',"
                                   "'"+current_lot_action+"',"
                                   "'"+change_master_sheet+"',"
                                   "'"+next_shift+"',"
                                   "'"+part_change_statue+"',"
                                   "'"+after_lot_monitering+"',"
                                   "'"+img_download_file+"',"
                                   "'"+attach_file_list+"',"
                                   "'2'"
                                   ");");
        query.exec(insert_query);
}
