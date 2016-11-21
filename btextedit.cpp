#include "btextedit.h"
#include <eismain.h>
#include "ui_eismain.h"
#include "ui_eis_listview_item.h"
#include "ui_eis_alarmlistview_item.h"

BTextEdit::BTextEdit(QWidget *parent) : QTextEdit(parent)
{


}
BTextEdit::BTextEdit(int *doc_number, QWidget *parent):QTextEdit(parent)
{
    this->setTextInteractionFlags(Qt::TextEditable|Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
    parent_widget = parent;
    this->doc_number = doc_number;
    connect(this,SIGNAL(cursorPositionChanged())
                 ,this,SLOT(b_text_cursor_change()));
}

void BTextEdit::insertFromMimeData( const QMimeData *source ){

          QString source_text = source->text();
          QString doc_number_txt = QString("%1").arg(*doc_number);
          QList<QUrl> source_list = source->urls();
          //qDebug()<<source->formats();
          QString makedir_txt;
          if(this->objectName()=="content_edit"){
                makedir_txt = qApp->applicationDirPath()+"/temp/EIS/img";
          }else if(this->objectName()=="alarm_content_edit"){
                makedir_txt = qApp->applicationDirPath()+"/temp/EIS/alarmimg";
          }
          qDebug()<<this->objectName();

          if(source->hasText() && source->hasText()){
            eis_select_copymode_dialog select_dialog;
            int result = select_dialog.exec();
            if(result == QDialog::Accepted){
                if(select_dialog.mode==PICTURE_MODE){
                    QImage image = qvariant_cast<QImage>(source->imageData());
                    D_image_size image_size_dialog;
                    image_size_dialog.setImg(image);
                    image_size_dialog.setHeight(image.size().height());
                    image_size_dialog.setWidth(image.size().width());
                    image_size_dialog.image_pack();
                    if(!image_size_dialog.exec()==QDialog::Accepted){
                        return;
                    }

                    QDir makedir(makedir_txt);
                    if(!makedir.exists(doc_number_txt)){
                       makedir.mkdir(doc_number_txt);
                    }

                    QString output_filename = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss_") + "chapture.png";
                    QString des_file = QString("%1/%2/%3").arg(makedir_txt).arg(doc_number_txt).arg(output_filename);
                    image.save(des_file,"PNG");
                    QTextCursor cursor = this->textCursor();
                    QTextImageFormat imageformat;
                    imageformat.setName(QString("%1%2").arg("file:///").arg(des_file));
                    imageformat.setHeight(image_size_dialog.getHeight());
                    imageformat.setWidth(image_size_dialog.getWidth());
                    cursor.insertImage(imageformat);
                    image_list.append(output_filename);

                }else if(select_dialog.mode == TEXT_MODE) {
                    QTextCursor cursor = this->textCursor();
                    cursor.insertText(source->text());
                }
            }else {
                return ;
            }

          }else if(source->hasImage()){
              QImage image = qvariant_cast<QImage>(source->imageData());
              D_image_size image_size_dialog;
              image_size_dialog.setImg(image);
              image_size_dialog.setHeight(image.size().height());
              image_size_dialog.setWidth(image.size().width());
              image_size_dialog.image_pack();
              if(!image_size_dialog.exec()==QDialog::Accepted){
                  return;
              }

              QDir makedir(makedir_txt);
              if(!makedir.exists(doc_number_txt)){
                 makedir.mkdir(doc_number_txt);
              }

              QString output_filename = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss_") + "chapture.png";
              QString des_file = QString("%1/%2/%3").arg(makedir_txt).arg(doc_number_txt).arg(output_filename);
              image.save(des_file,"PNG");
              QTextCursor cursor = this->textCursor();
              QTextImageFormat imageformat;
              imageformat.setName(QString("%1%2").arg("file:///").arg(des_file));
              imageformat.setHeight(image_size_dialog.getHeight());
              imageformat.setWidth(image_size_dialog.getWidth());
              cursor.insertImage(imageformat);
              image_list.append(output_filename);
          }else if(ispicture(source_text)){
                if(source_text.indexOf('%',0)>0){
                    QMessageBox msg;
                    msg.setText(tr("no special input"));
                    msg.addButton(QMessageBox::Ok);
                    msg.exec();
                    return ;
                }
                QFile source_file(source_list.at(0).toString(QUrl::PreferLocalFile));

                QDir makedir(makedir_txt);
                if(!makedir.exists(doc_number_txt)){
                   makedir.mkdir(doc_number_txt);
                }
                QString output_file = outputfilename(source_list.at(0).toString(QUrl::PreferLocalFile));
                QString des_file = QString("%1/%2/%3").arg(makedir_txt).arg(doc_number_txt).arg(output_file);
                source_file.copy(des_file);

                QImage tempimage;
                tempimage.load(des_file);
                D_image_size image_size_dialog;
                image_size_dialog.setFilepath(des_file);
                image_size_dialog.setHeight(tempimage.size().height());
                image_size_dialog.setWidth(tempimage.size().width());
                image_size_dialog.image_pack();
                if(!image_size_dialog.exec()==QDialog::Accepted){
                    QFile temp_des_file(des_file);
                    temp_des_file.remove();
                    return;
                }

                QTextCursor cursor = this->textCursor();
                QTextImageFormat imageformat;
                imageformat.setName(QString("%1%2").arg("file:///").arg(des_file));
                imageformat.setHeight(image_size_dialog.getHeight());
                imageformat.setWidth(image_size_dialog.getWidth());
                cursor.insertImage(imageformat);
                image_list.append(des_file);
          }else {
              QTextEdit::insertFromMimeData(source);
          }

}
void BTextEdit::replyFinished(QNetworkReply* reply){
    qDebug()<<"finish";
}

void BTextEdit::b_text_cursor_change()
{
    if(textCursor().selection().toPlainText()==""){
        QString objname = parent_widget->objectName();
        if(objname == "EISmain"){

            EISmain *eismain = (EISmain *)parent_widget;
            if(eismain->ui->tabWidget->currentIndex()==1){
                int size = currentCharFormat().font().pointSize();
                QString font_family =currentCharFormat().font().family();
                eismain->ui->fontsize->setValue(size);
                eismain->ui->font_type->setCurrentText(font_family);
                if(currentCharFormat().font().bold()){
                    eismain->ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                                "	border-image:url(:/img/img/Bold_btn_activity.png)\n"
                                                "}"));
                }else {
                    eismain->ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                                "	border-image:url(:/img/img/Bold_btn_inactivity.png)\n"
                                                "}"));

                }
                if(currentCharFormat().font().underline()){
                    eismain->ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                                "	border-image:url(:/img/img/under_line_activity.png)\n"
                                                "}"));
                }else {
                    eismain->ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                                "	border-image:url(:/img/img/under_line_inactivity.png)\n"
                                                "}"));

                }
                QColor font_color = currentCharFormat().foreground().color();
                eismain->ui->now_color->setStyleSheet(QString("background-color : %1").arg(font_color.name()));
            }else if(eismain->ui->tabWidget->currentIndex()==2) {
                int size = currentCharFormat().font().pointSize();
                QString font_family =currentCharFormat().font().family();
                eismain->ui->fontsize_2->setValue(size);
                eismain->ui->font_type_2->setCurrentText(font_family);
                if(currentCharFormat().font().bold()){
                    eismain->ui->Bold_btn_2->setStyleSheet(QLatin1String("QPushButton{\n"
                                                "	border-image:url(:/img/img/Bold_btn_activity.png)\n"
                                                "}"));
                }else {
                    eismain->ui->Bold_btn_2->setStyleSheet(QLatin1String("QPushButton{\n"
                                                "	border-image:url(:/img/img/Bold_btn_inactivity.png)\n"
                                                "}"));

                }
                if(currentCharFormat().font().underline()){
                    eismain->ui->underline_btn_2->setStyleSheet(QLatin1String("QPushButton{\n"
                                                "	border-image:url(:/img/img/under_line_activity.png)\n"
                                                "}"));
                }else {
                    eismain->ui->underline_btn_2->setStyleSheet(QLatin1String("QPushButton{\n"
                                                "	border-image:url(:/img/img/under_line_inactivity.png)\n"
                                                "}"));

                }
                QColor font_color = currentCharFormat().foreground().color();
                eismain->ui->now_color_2->setStyleSheet(QString("background-color : %1").arg(font_color.name()));
            }
        }else if (objname == "EIS_listview_item"){
            EIS_listview_item *eis_listview_item = (EIS_listview_item *)parent_widget;
            int size = currentCharFormat().font().pointSize();
            QString font_family =currentCharFormat().font().family();
            eis_listview_item->ui->fontsize->setValue(size);
            eis_listview_item->ui->font_type->setCurrentText(font_family);
            if(currentCharFormat().font().bold()){
                eis_listview_item->ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                            "	border-image:url(:/img/img/Bold_btn_activity.png)\n"
                                            "}"));
            }else {
                eis_listview_item->ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                            "	border-image:url(:/img/img/Bold_btn_inactivity.png)\n"
                                            "}"));

            }
            if(currentCharFormat().font().underline()){
                eis_listview_item->ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                            "	border-image:url(:/img/img/under_line_activity.png)\n"
                                            "}"));
            }else {
                eis_listview_item->ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                            "	border-image:url(:/img/img/under_line_inactivity.png)\n"
                                            "}"));

            }
            QColor font_color = currentCharFormat().foreground().color();
            eis_listview_item->ui->now_color->setStyleSheet(QString("background-color : %1").arg(font_color.name()));

        }else if (objname == "eis_alarmlistview_item"){
            eis_alarmlistview_item *eis_alarmlistview = (eis_alarmlistview_item *)parent_widget;
            int size = currentCharFormat().font().pointSize();
            QString font_family =currentCharFormat().font().family();
            eis_alarmlistview->ui->fontsize->setValue(size);
            eis_alarmlistview->ui->font_type->setCurrentText(font_family);
            if(currentCharFormat().font().bold()){
                eis_alarmlistview->ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                            "	border-image:url(:/img/img/Bold_btn_activity.png)\n"
                                            "}"));
            }else {
                eis_alarmlistview->ui->Bold_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                            "	border-image:url(:/img/img/Bold_btn_inactivity.png)\n"
                                            "}"));

            }
            if(currentCharFormat().font().underline()){
                eis_alarmlistview->ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                            "	border-image:url(:/img/img/under_line_activity.png)\n"
                                            "}"));
            }else {
                eis_alarmlistview->ui->underline_btn->setStyleSheet(QLatin1String("QPushButton{\n"
                                            "	border-image:url(:/img/img/under_line_inactivity.png)\n"
                                            "}"));

            }
            QColor font_color = currentCharFormat().foreground().color();
            eis_alarmlistview->ui->now_color->setStyleSheet(QString("background-color : %1").arg(font_color.name()));
        }
    }
}
/**
 * @brief BTextEdit::ispicture
 * 그림 파일 유무 판별
 * @param source
 * @return
 */
bool BTextEdit::ispicture(QString source)
{
    QStringList picturesoure;
    bool result = false;
    picturesoure.append(".png");
    picturesoure.append(".jpg");
    picturesoure.append(".bmp");
    picturesoure.append(".jpeg");
    picturesoure.append(".gif");
    for(int i=0;i<picturesoure.count();i++){
        if(source.indexOf(picturesoure.at(i))>0){
            result = true;
        }
    }
    return result;
}

/**
 * @brief BTextEdit::outputfilename
 * output_file_name 생성
 * @param source
 * @return
 */
QString BTextEdit::outputfilename(QString source)
{
    QString result;
    result = source.split("/").last();
    QString nowdatetime = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss_");
    result = nowdatetime + result;
    return result;
}

QString BTextEdit::tosqlhtml()
{
    QString reslut = document()->toHtml();
    reslut = reslut.replace("\'","\\'");
    return reslut;
}

QStringList BTextEdit::getImage_list() const
{
    return image_list;
}


void BTextEdit::setImage_list(const QStringList &value)
{
    image_list = value;
}


