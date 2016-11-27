#include "hanile_log_main.h"
#include "ui_hanile_log_main.h"

hanile_log_main::hanile_log_main(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hanile_log_main)
{
    ui->setupUi(this);
}

hanile_log_main::~hanile_log_main()
{
    delete ui;
}

void hanile_log_main::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void hanile_log_main::dropEvent(QDropEvent *event)
{
    QMimeData *source = (QMimeData *)event->mimeData();
    if(source->hasUrls()){
        QList<QUrl> urls = source->urls();
        for(int i=0;i<urls.size();i++){
            QFileInfo fileinfo(urls.at(i).toLocalFile());
            if(fileinfo.isDir()){
                QDir tempdir(urls.at(i).toLocalFile());
                QFileInfoList enrtylist = tempdir.entryInfoList(QDir::Files);
                for(int j=0;j<enrtylist.count();j++){
                    hanile_log_item *item = new hanile_log_item();
                    item->setFile_path(enrtylist.at(j).absoluteFilePath());
                    item->file_read();
                    item_list.append(item);
                }
            }else {
                    hanile_log_item *item = new hanile_log_item();
                    item->setFile_path(urls.at(i).toLocalFile());
                    item->file_read();
                    item_list.append(item);
            }
        }
    }
    for(int i=0;i<item_list.count();i++){

    }
    for(int i=0;i<item_list.count();i++){
        qDebug()<<item_list.at(i)->item_list.count();
        for(int j=0;j<item_list.at(i)->item_list.count();j++){
            int count = ui->item_table->rowCount();
            ui->item_table->insertRow(count);
            QLabel *label = item_list.at(i)->item_list.at(j)->getLA_filename();
            qDebug()<<label->text();
            ui->item_table->setCellWidget(count,0,item_list.at(i)->item_list.at(j)->getLA_filename());
            ui->item_table->setCellWidget(count,1,item_list.at(i)->item_list.at(j)->getLA_filetime());
            ui->item_table->setCellWidget(count,2,item_list.at(i)->item_list.at(j)->getLA_layernumber());
            ui->item_table->setCellWidget(count,3,item_list.at(i)->item_list.at(j)->getLA_Matname());
        }
    }

}
