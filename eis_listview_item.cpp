#include "eis_listview_item.h"
#include "ui_eis_listview_item.h"
#include <eis_list_view.h>
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

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand(QString("CWD /home/eis/img/%1").arg(doc_number));
    loop.exec();
    makedir_txt = qApp->applicationDirPath()+"/temp/EIS/img/"+QString("%1").arg(doc_number);
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

    query_txt = QString("select facilities from People_information where factory_process = '%1' group by facilities order by facilities asc")
            .arg(query_base_data.value("factory_process").toString());
    query.exec(query_txt);
    while(query.next()){
        ui->select_facilities->addItem(query.value("facilities").toString());
    }
    ui->select_facilities->setCurrentText(query_base_data.value("facilities_name").toString());

    query_txt = QString("select name from People_information where factory_process = '%1' group by name order by name asc")
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

    content_edit = new BTextEdit(&doc_number,this);
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

    if(query_base_data.value("complete").toInt() == 3){
        ui->complete->setChecked(true);
    }else {
        ui->no_complete->setChecked(true);
    }
    QString content_change = query_base_data.value("content").toString();
    content_change.replace("server_image_change_part_temp",qApp->applicationDirPath());
    content_edit->setHtml(content_change);

    QSqlQuery query_history_data(db);
    query_txt = QString("select write_time,witer_name from EIS_document where idx = '%1' ORDER BY `write_time` desc").arg(doc_number_str);
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


}


EIS_listview_item::~EIS_listview_item()
{
    ftp->deleteLater();
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
    ui->select_facilities->clear();
    ui->select_facilities->addItem("");
    QSqlQuery query(db);
    QString query_txt = QString("select facilities from People_information where factory_process = '%1' group by facilities").arg(arg1);
    query.exec(query_txt);
    while(query.next()){
        ui->select_facilities->addItem(query.value("facilities").toString());
    }

    ui->select_name->clear();
    ui->select_name->addItem("");
    query_txt = QString("select name from People_information where factory_process = '%1' group by name order by name asc").arg(arg1);
    query.exec(query_txt);
    while(query.next()){
        ui->select_name->addItem(query.value("name").toString());
    }
}

void EIS_listview_item::on_select_facilities_currentIndexChanged(const QString &arg1)
{
//    ui->select_name->clear();;
//    ui->select_name->addItem("");
//    QSqlQuery query(db);
//    QString query_txt = QString("select name from People_information where facilities = '%1'").arg(arg1);
//    query.exec(query_txt);
//    while(query.next()){
//        ui->select_name->addItem(query.value("name").toString());
//    }
}


void EIS_listview_item::on_font_type_currentTextChanged(const QString &arg1)
{
    mainfont.setFamily(arg1);
    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    content_edit->setCurrentCharFormat(charfotmet);

}

void EIS_listview_item::on_fontsize_editingFinished()
{
    mainfont.setPointSize(ui->fontsize->value());
    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    content_edit->setCurrentCharFormat(charfotmet);

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

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand(QString("CWD /home/eis/attach/%1").arg(doc_number));
    loop.exec();
    save_file->open(QFile::ReadWrite);
    ftp->get(source_file,save_file);
    progressdialog->setLabelText("download");
    progressdialog->exec();
    save_file->close();
}

void EIS_listview_item::on_total_view_content_clicked()
{
    EIS_big_view *big_view = new EIS_big_view(ui->content_label->text(),content_edit,ui->font_type,
                                              ui->fontsize,ui->Bold_btn,ui->underline_btn,
                                              ui->now_color,ui->color_dialog,
                                              ui->layout_content,ui->font_layout);
        big_view->show();
}



void EIS_listview_item::on_modify_button_clicked()
{
    if(ftp->state()==QFtp::Unconnected){
        ftp->connectToHost(server_ip,21);

        ftp->login(QUrl::fromPercentEncoding(FTPID),FTPPW);
        loop.exec();
        ftp->setTransferMode(QFtp::Passive);
    }
    ftp->rawCommand("CWD /home/eis/img");
    loop.exec();
    ftp->rawCommand(QString("MKD %1").arg(doc_number));
    loop.exec();
    ftp->rawCommand(QString("CWD /home/eis/img/%1").arg(doc_number));
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
    QString complete;
    if(ui->complete->isChecked()){
        complete = "3";
        QSqlQuery update_query(db);
        QString query_txt = QString("update EIS_document SET `complete`='3' WHERE  `idx`=%1").arg(doc_number);
        update_query.exec(query_txt);
    }else {
        QSqlQuery update_query(db);
        QString query_txt = QString("update EIS_document SET `complete`='2' WHERE  `idx`=%1").arg(doc_number);
        update_query.exec(query_txt);
        complete = "2";
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
                                   "`content`,"
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
                                   "'"+content+"',"
                                   "'"+img_download_file+"',"
                                   "'"+attach_file_list+"',"
                                   "'"+complete+"'"
                                   ");");
        query.exec(insert_query);
        QMessageBox msg;
        msg.addButton(QMessageBox::Ok);
        msg.setText("modify complete");

        msg.exec();
}


void EIS_listview_item::on_print_button_clicked()
{
    QPrinter printer;
    printer.setPaperSize(QPrinter::A4);
    printer.setFullPage(true);

    QPrintDialog *dialog= new QPrintDialog(&printer);
    int result = dialog->exec();
    if(result != QPrintDialog::Accepted){
        return;
    }

    QTextEdit edit;

    QTextTable *table1 = edit.textCursor().insertTable(2,6);

    table1->cellAt(0,0).firstCursorPosition().insertText(tr("team"));
    table1->cellAt(1,0).firstCursorPosition().insertText(ui->select_team->currentText());
    table1->cellAt(0,1).firstCursorPosition().insertText(tr("process"));
    table1->cellAt(1,1).firstCursorPosition().insertText(ui->select_process->currentText());
    table1->cellAt(0,2).firstCursorPosition().insertText(tr("facilities"));
    table1->cellAt(1,2).firstCursorPosition().insertText(ui->select_facilities->currentText());
    table1->cellAt(0,3).firstCursorPosition().insertText(tr("witer_name"));
    table1->cellAt(1,3).firstCursorPosition().insertText(ui->select_name->currentText());
    table1->cellAt(0,4).firstCursorPosition().insertText(tr("change_have"));
    if(ui->radio_btn_change_exist->isChecked()){
        table1->cellAt(1,4).firstCursorPosition().insertText(tr("have"));
    }else {
        table1->cellAt(1,4).firstCursorPosition().insertText(tr("don't have"));
    }
    table1->cellAt(0,5).firstCursorPosition().insertText(tr("complete"));
    if(ui->complete->isChecked()){
        table1->cellAt(1,5).firstCursorPosition().insertText(tr("have"));
    }else {
        table1->cellAt(1,5).firstCursorPosition().insertText(tr("don't have"));
    }
    edit.textCursor().insertText("\n");

    QTextTable *table2 = edit.textCursor().insertTable(1,2);
    table2->cellAt(0,0).firstCursorPosition().insertText(tr("subname"));
    table2->cellAt(0,1).firstCursorPosition().insertText(ui->document_name->text());

    edit.textCursor().insertHtml(content_edit->toHtml());
    edit.print(&printer);

    dialog->deleteLater();
}

void EIS_listview_item::on_table_save_histroy_cellDoubleClicked(int row, int column)
{
         QLabel *LE_time = (QLabel *)ui->table_save_histroy->cellWidget(row,0);
         QStringList check_list;
         QString number_doc = QString("%1").arg(doc_number);
         number_doc = number_doc + "/"+LE_time->text();
         check_list.append(number_doc);
         Eis_list_view *list_view = new Eis_list_view(check_list);
         list_view->modify_button_show(false);
         list_view->attach_button_show(false);
         list_view->show();
}

void EIS_listview_item::on_attach_btn_clicked()
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
        ftp->put(source_file,upload_filename);
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

void EIS_listview_item::on_attach_remove_btn_clicked()
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

void EIS_listview_item::on_Bold_btn_clicked()
{
    QString current_stylesheet =  ui->Bold_btn->styleSheet();
    bool current_bold;
    if(current_stylesheet.indexOf("Bold_btn_activity.png")>0){
        current_bold = true;
    }else {
        current_bold = false;
    }
    if(current_bold){
        mainfont.setBold(false);
        ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/Bold_btn_inactivity.png)\n"
                                    "}"));
    }else {
        mainfont.setBold(true);
        ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/Bold_btn_activity.png)\n"
                                    "}"));
    }
    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    content_edit->setCurrentCharFormat(charfotmet);
}

void EIS_listview_item::on_underline_btn_clicked()
{
    QString current_stylesheet =  ui->underline_btn->styleSheet();
    bool current_underline;
    if(current_stylesheet.indexOf("under_line_activity.png")>0){
        current_underline = true;
    }else {
        current_underline = false;
    }
    if(current_underline){
        mainfont.setUnderline(false);
        ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/under_line_inactivity.png)\n"
                                    "}"));
    }else {
        mainfont.setUnderline(true);
        ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                    "	border-image:url(:/img/img/under_line_activity.png)\n"
                                    "}"));
    }
    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    content_edit->setCurrentCharFormat(charfotmet);
}
void EIS_listview_item::on_color_dialog_clicked()
{
   QColor font_color = QColorDialog::getColor();
   QTextCharFormat charfotmet;
   charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
   charfotmet.setForeground(QBrush(font_color));
   content_edit->setCurrentCharFormat(charfotmet);
}
