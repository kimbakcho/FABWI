#include "eis_alarmlistview_item.h"
#include "ui_eis_alarmlistview_item.h"

eis_alarmlistview_item::eis_alarmlistview_item(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::eis_alarmlistview_item)
{
    ui->setupUi(this);
}

eis_alarmlistview_item::eis_alarmlistview_item(QString doc_data,QWidget *parent):
    QWidget(parent),
    ui(new Ui::eis_alarmlistview_item)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    progressdialog = 0;
    QString doc_number_str = doc_data.split("/").at(0);
    QString doc_write_time = doc_data.split("/").at(1);
    doc_number =doc_number_str.toInt();
    QString configini_str = qApp->applicationDirPath()+"/server.ini";

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

    if(!db.contains("EISDB")){
       db = QSqlDatabase::addDatabase("QMYSQL","EISDB");
       db.setHostName(server_ip);
       db.setPort(db_port.toInt());
       db.setUserName(DBID);
       db.setPassword(DBPW);
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
    QString query_txt = QString("select * from EIS_alarm_document where idx = \'%1\' AND write_time = \'%2\'")
            .arg(doc_number_str).arg(doc_write_time);
    query_base_data.exec(query_txt);
    query_base_data.next();

    QString makedir_txt = qApp->applicationDirPath()+"/temp/EIS/alarmimg";

    QDir makedir(makedir_txt);
    if(!makedir.exists(doc_number_str)){
       makedir.mkdir(doc_number_str);
    }

    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand(QString("CWD /home/eis/alarmimg/%1").arg(doc_number));
    loop.exec();
    makedir_txt = qApp->applicationDirPath()+"/temp/EIS/alarmimg/"+QString("%1").arg(doc_number);
    img_download_file = query_base_data.value("downloadimg").toString();
    QStringList img_file_list = query_base_data.value("downloadimg").toString().split("/////");
    for(int i=0;i<img_file_list.count()-1;i++){
        if(progressdialog==0){
            progressdialog = new QProgressDialog(this);
        }
        QString file_name = img_file_list.at(i);
        QString des_file_path = makedir_txt+"/"+file_name;
        QFile des_file(des_file_path);
        des_file.open(QFile::ReadWrite);
        ftp->get(file_name,&des_file);
        progressdialog->exec();
        des_file.close();
    }
    QSqlQuery query(db);

    disconnect(ui->select_team,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(on_select_team_currentIndexChanged(QString)));
    disconnect(ui->select_process,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(on_select_process_currentIndexChanged(QString)));
    disconnect(ui->select_facilities,SIGNAL(currentIndexChanged(QString)),
               this,SLOT(on_select_facilities_currentIndexChanged(QString)));
    query.exec("select team from People_information GROUP by team;");

    ui->select_team->addItem("");
    while(query.next()){
        ui->select_team->addItem(query.value("team").toString());
    }
    ui->select_team->setCurrentText(query_base_data.value("team").toString());

    ui->select_process->clear();
    ui->select_process->addItem("");
    query_txt = QString("select factory_process from People_information where team = '%1' group by factory_process")
            .arg(query_base_data.value("team").toString());
    query.exec(query_txt);
    while(query.next()){
        ui->select_process->addItem(query.value("factory_process").toString());
    }
    ui->select_process->setCurrentText(query_base_data.value("factory_process").toString());

    ui->select_facilities->clear();
    ui->select_facilities->addItem("");
    query_txt = QString("select facilities from People_information where factory_process = '%1' group by facilities order by facilities asc")
            .arg(query_base_data.value("factory_process").toString());
    query.exec(query_txt);
    while(query.next()){
        ui->select_facilities->addItem(query.value("facilities").toString());
    }
    ui->select_facilities->setCurrentText(query_base_data.value("facilities_name").toString());

    ui->select_name->clear();
    ui->select_name->addItem("");
    query_txt = QString("select name from People_information where team = '%1' group by name order by name asc")
            .arg(query_base_data.value("team").toString());
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

    content_edit = new BTextEdit(&doc_number,this);
    content_edit->setObjectName("alarm_content_edit");
    ui->layout_content->addWidget(content_edit,0,0);

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
    content_edit->setCurrentCharFormat(charfotmet);

    ui->current_time->setText(query_base_data.value("write_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

    ui->document_name->setText(query_base_data.value("document_name").toString());

    QString content_change = query_base_data.value("content").toString();
    content_change.replace("server_image_change_part_temp",qApp->applicationDirPath());
    content_edit->setHtml(content_change);

    QSqlQuery query_history_data(db);
    query_txt = QString("select write_time,witer_name from EIS_alarm_document where idx = '%1' ORDER BY `write_time` desc").arg(doc_number_str);
    query_history_data.exec(query_txt);
    ui->table_save_histroy->horizontalHeader()->resizeSection(0,150);
    while(query_history_data.next()){
        int row_count = ui->table_save_histroy->rowCount();
        ui->table_save_histroy->insertRow(row_count);
        ui->table_save_histroy->setCellWidget(row_count,0,new QLabel(query_history_data
                                                                     .value("write_time")
                                                                     .toDateTime()
                                                                     .toString("yyyy-MM-dd hh:mm:ss")));
        ui->table_save_histroy->setCellWidget(row_count,1,new QLabel(query_history_data.value("witer_name")
                                                                     .toString()));
    }

    if(query_base_data.value("priority").toInt() == 1){
       ui->priority_low->setChecked(true);
    }else if(query_base_data.value("priority").toInt() == 2){
       ui->priority_middle->setChecked(true);
    }else if(query_base_data.value("priority").toInt() == 3){
       ui->priority_high->setChecked(true);
    }else if(query_base_data.value("priority").toInt() == 4){
       ui->priority_very_high->setChecked(true);
    }


}
void eis_alarmlistview_item::on_select_team_currentIndexChanged(const QString &arg1)
{
    QString query_txt;
    QSqlQuery query(db);
    if(arg1!=""){
        ui->select_process->clear();
        ui->select_process->addItem("");
        query_txt = QString("select factory_process from People_information where team = '%1' group by factory_process order by factory_process asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_process->addItem(query.value("factory_process").toString());
        }
        ui->select_facilities->clear();
        ui->select_facilities->addItem("");
        query_txt = QString("select facilities from People_information where team = '%1' group by facilities order by facilities asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities->addItem(query.value("facilities").toString());
        }
        ui->select_name->clear();
        ui->select_name->addItem("");
        query_txt = QString("select name from People_information where team = '%1' group by name order by name asc").arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_name->addItem(query.value("name").toString());
        }
    }else {
        ui->select_process->clear();
        ui->select_process->addItem("");
        query_txt = QString("select factory_process from People_information group by factory_process order by factory_process asc");
        query.exec(query_txt);
        while(query.next()){
            ui->select_process->addItem(query.value("factory_process").toString());
        }
        ui->select_facilities->clear();
        ui->select_facilities->addItem("");
        query_txt = QString("select facilities from People_information group by facilities order by facilities asc");
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities->addItem(query.value("facilities").toString());
        }
        ui->select_name->clear();
        ui->select_name->addItem("");
        query_txt = QString("select name from People_information group by name order by name");
        query.exec(query_txt);
        while(query.next()){
            ui->select_name->addItem(query.value("name").toString());
        }
    }
}

void eis_alarmlistview_item::on_select_process_currentIndexChanged(const QString &arg1)
{
    QSqlQuery query(db);
    QString query_txt;
    if(arg1!=""){
        ui->select_facilities->clear();
        ui->select_facilities->addItem("");
        QString search;
        if(ui->select_team->currentText()!=""){
            search = QString("team = '%1' AND").arg(ui->select_team->currentText());
        }else {
            search = "";
        }
        query_txt = QString("select facilities from People_information where %1 factory_process = '%2' "
                            "group by facilities order by facilities asc").arg(search).arg(arg1);
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities->addItem(query.value("facilities").toString());
        }
    }else {
        ui->select_facilities->clear();
        ui->select_facilities->addItem("");
        QString search;
        if(ui->select_team->currentText()!=""){
            search = QString("where team = '%1' AND").arg(ui->select_team->currentText());
        }else {
            search = "";
        }

        query_txt = QString("select facilities from People_information %1 "
                            "group by facilities order by facilities asc").arg(search);
        query.exec(query_txt);
        while(query.next()){
            ui->select_facilities->addItem(query.value("facilities").toString());
        }
    }
}

void eis_alarmlistview_item::on_select_facilities_currentIndexChanged(const QString &arg1)
{

}
void eis_alarmlistview_item::on_font_type_currentTextChanged(const QString &arg1)
{
    QFont update_font;
    update_font = content_edit->currentCharFormat().font();
    update_font.setFamily(arg1);
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(content_edit->currentCharFormat().foreground());
    content_edit->setCurrentCharFormat(charfotmet);

}

void eis_alarmlistview_item::on_fontsize_editingFinished()
{
    QFont update_font;
    update_font = content_edit->currentCharFormat().font();
    update_font.setPointSize(ui->fontsize->value());
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(content_edit->currentCharFormat().foreground());
    content_edit->setCurrentCharFormat(charfotmet);

}
void eis_alarmlistview_item::ftpCommandFinished(int commandId, bool error)
{

    loop.exit();
}

void eis_alarmlistview_item::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    if(totalBytes==0){
        return;
    }
    progressdialog->setMaximum(totalBytes);
    progressdialog->setValue(readBytes);
}

void eis_alarmlistview_item::ftp_listInfo(QUrlInfo urlInfo)
{
    qDebug()<<urlInfo.name();
}

void eis_alarmlistview_item::ftp_rawCommandReply(int a, QString data)
{
       //qDebug()<<"ftp data = "<<data;
}

void eis_alarmlistview_item::on_attach_listview_doubleClicked(const QModelIndex &index)
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

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand(QString("CWD /home/eis/alarmattach/%1").arg(doc_number));
    loop.exec();
    save_file->open(QFile::ReadWrite);
    ftp->get(source_file,save_file);
    if(progressdialog==0){
        progressdialog = new QProgressDialog(this);
    }
    progressdialog->setLabelText("download");
    progressdialog->exec();
    save_file->close();
}
void eis_alarmlistview_item::on_total_view_content_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->content_label->text(),content_edit,ui->font_type,
                                              ui->fontsize,ui->Bold_btn,ui->underline_btn,
                                              ui->now_color,ui->color_dialog,
                                              ui->layout_content,ui->font_layout);
        big_view->show();
}

void eis_alarmlistview_item::on_modify_button_clicked()
{
    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand("CWD /home/eis/alarmimg");
    loop.exec();
    ftp->rawCommand(QString("MKD %1").arg(doc_number));
    loop.exec();
    ftp->rawCommand(QString("CWD /home/eis/alarmimg/%1").arg(doc_number));
    loop.exec();

    QString makedir_txt = qApp->applicationDirPath()+"/temp/EIS/alarmimg/"+QString("%1").arg(doc_number);
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

    QSqlQuery query(db);

    QString document_name = ui->document_name->text();
    QString team_name = ui->select_team->currentText();
    QString process = ui->select_process->currentText();
    QString write_name = ui->select_name->currentText();
    QString facilities_name = ui->select_facilities->currentText();
    QString content_change = content_edit->tosqlhtml();

    content_change.replace(qApp->applicationDirPath(),"server_image_change_part_temp");
    QString content = content_change;

    QString attach_file_list;
    QStringList total_img_list;
    QString total_img_txt;

    for(int i=0;i<content_edit->image_list.count();i++){
        QString img_path = content_edit->image_list.at(i);
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
    QString priority_value;
    if(ui->priority_low->isChecked()){
        priority_value = "1";
    }else if(ui->priority_middle->isChecked()){
        priority_value = "2";
    }else if(ui->priority_high->isChecked()){
        priority_value = "3";
    }else if(ui->priority_very_high->isChecked()){
        priority_value = "4";
    }
    QString insert_query = QString("INSERT INTO EIS_alarm_document ("
                           "`idx`, "
                           "`team`, "
                           "`write_time`, "
                           "`factory_process`, "
                           "`facilities_name`, "
                           "`witer_name`, "
                           "`document_name`, "
                           "`content`, "
                           "`downloadimg`, "
                           "`attach_file_list`,"
                           "`priority`"
                           ") "
                           "VALUES ("
                           "'"+QString("%1").arg(doc_number)+"',"
                           "'"+team_name+"',"
                           "'"+now_datetime+"',"
                           "'"+process+"',"
                           "'"+facilities_name+"',"
                           "'"+write_name+"',"
                           "'"+document_name+"',"
                           "'"+content+"',"
                           "'"+img_download_file+"',"
                           "'"+attach_file_list+"',"
                           "'"+priority_value+"'"
                           ")");
        query.exec(insert_query);
        QMessageBox msg;
        msg.addButton(QMessageBox::Ok);
        msg.setText(tr("modify complete"));

        msg.exec();
}
void eis_alarmlistview_item::on_table_save_histroy_cellDoubleClicked(int row, int column)
{
         QLabel *LE_time = (QLabel *)ui->table_save_histroy->cellWidget(row,0);
         QStringList check_list;
         QString number_doc = QString("%1").arg(doc_number);
         number_doc = number_doc + "/"+LE_time->text();
         check_list.append(number_doc);
         eis_alarmlistview_item *view = new eis_alarmlistview_item(number_doc);
         view->ui->modify_button->setVisible(false);
         view->ui->attach_btn->setVisible(false);
         view->ui->attach_remove_btn->setVisible(false);
         view->show();
}
void eis_alarmlistview_item::on_attach_btn_clicked()
{
    QFileDialog filedialog(this);
    filedialog.setFileMode(QFileDialog::AnyFile);
    filedialog.setViewMode(QFileDialog::Detail);
    if(filedialog.exec()==QFileDialog::Accepted){
        QString filename = filedialog.selectedFiles().at(0);
        if(ftp->state()==QFtp::Unconnected){
            ftp->connectToHost(server_ip,21);

            ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
            loop.exec();
            ftp->setTransferMode(QFtp::Passive);
        }
        ftp->rawCommand("CWD /home/eis/attach");
        loop.exec();
        ftp->rawCommand(QString("MKD %1").arg(doc_number));
        loop.exec();
        ftp->rawCommand(QString("CWD /home/eis/attach/%1").arg(doc_number));
        loop.exec();
        QFile *source_file = new QFile(filename);
        source_file->open(QFile::ReadWrite);
        QString upload_filename = filename.split('/').last();
        for(int i=0;i<attach_list_model->rowCount();i++){
              QString item = attach_list_model->item(i)->text();
              if(item == upload_filename){
                    QString first_name = upload_filename.split(".").at(0)+"_t";
                    QString Last_name;
                    if(upload_filename.split('.').size()>=2){
                        Last_name = "."+upload_filename.split(".").at(1);
                    }
                    upload_filename =first_name + Last_name;
              }
        }
        ftp->put(source_file,upload_filename.toUtf8());
        if(progressdialog == 0){
            progressdialog = new QProgressDialog(this);
        }
        progressdialog->setLabelText("upload");
        progressdialog->exec();
        attach_list_model->insertRow(attach_list_model->rowCount(),
                                     new QStandardItem(upload_filename));
        source_file->close();
    }
}
void eis_alarmlistview_item::on_attach_remove_btn_clicked()
{
    QModelIndex index  = ui->attach_listview->currentIndex();
//    QString select_file = index.data().toString();
//    if(ftp->state()==QFtp::Unconnected){
//        ftp->connectToHost(server_ip,21);

//        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
//        loop.exec();
//        ftp->setTransferMode(QFtp::Passive);
//    }
//    ftp->rawCommand(QString("CWD /home/eis/attach/%1").arg(doc_number));
//    loop.exec();
//    ftp->remove(select_file);
//    loop.exec();
    attach_list_model->removeRow(index.row());
}
void eis_alarmlistview_item::on_Bold_btn_clicked()
{
    QString current_stylesheet =  ui->Bold_btn->styleSheet();
    bool current_bold;
    if(current_stylesheet.indexOf("Bold_btn_activity.png")>0){
        current_bold = true;
    }else {
        current_bold = false;
    }
    QFont update_font ;
    if(current_bold){

        update_font = content_edit->currentCharFormat().font();
        update_font.setBold(false);
        ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/Bold_btn_inactivity.png)\n"
                                    "}"));
    }else {
;
        update_font = content_edit->currentCharFormat().font();
        update_font.setBold(true);
        ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/Bold_btn_activity.png)\n"
                                    "}"));
    }
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(content_edit->currentCharFormat().foreground());
    content_edit->setCurrentCharFormat(charfotmet);
}

void eis_alarmlistview_item::on_underline_btn_clicked()
{
    QString current_stylesheet =  ui->underline_btn->styleSheet();
    bool current_underline;
    if(current_stylesheet.indexOf("under_line_activity.png")>0){
        current_underline = true;
    }else {
        current_underline = false;
    }
    QFont update_font ;
    if(current_underline){
        update_font = content_edit->currentCharFormat().font();
        update_font.setUnderline(false);
        ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/under_line_inactivity.png)\n"
                                    "}"));
    }else {
        update_font = content_edit->currentCharFormat().font();
        update_font.setUnderline(true);
        ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/under_line_activity.png)\n"
                                    "}"));
    }
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(content_edit->currentCharFormat().foreground());
    content_edit->setCurrentCharFormat(charfotmet);
}
void eis_alarmlistview_item::on_color_dialog_clicked()
{
    QColor font_color = QColorDialog::getColor();
    QFont update_font;
    update_font = content_edit->currentCharFormat().font();
    QTextCharFormat charfotmet;
    charfotmet.setFont(update_font,QTextCharFormat::FontPropertiesAll);
    charfotmet.setForeground(QBrush(font_color));
    content_edit->setCurrentCharFormat(charfotmet);
}
eis_alarmlistview_item::~eis_alarmlistview_item()
{
    delete ui;
}

void eis_alarmlistview_item::on_delete_btn_clicked()
{
    EIS_delete_dialog dialog;
    int reslut = dialog.exec();
    if(reslut == QDialog::Accepted && dialog.getPassword_alright()){
        QSqlQuery query(db);
        query.exec(QString("UPDATE `EIS_alarm_document` SET `delete_have`='1' where `idx`= %1").arg(doc_number));
        emit update_alarm_data();
        QMessageBox msg;
        msg.addButton(QMessageBox::Ok);
        msg.setText(tr("delete complete"));
        msg.exec();
        close();

    }else {

    }
}
