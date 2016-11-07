#include "btextedit.h"
#include <eismain.h>
#include "ui_eismain.h"
#include "ui_eis_listview_item.h"

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
          QString makedir_txt = qApp->applicationDirPath()+"/temp/EIS/img";

          if(source->hasImage()){
              QImage image = qvariant_cast<QImage>(source->imageData());
              D_image_size image_size_dialog;
              image_size_dialog.setHeight(image.size().height());
              image_size_dialog.setWidth(image.size().width());
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
          }

          if(ispicture(source_text)){

                QFile source_file(outputpath(source_text),this);
                QImage tempimage;
                tempimage.load(outputpath(source_text));
                D_image_size image_size_dialog;
                image_size_dialog.setHeight(tempimage.size().height());
                image_size_dialog.setWidth(tempimage.size().width());
                if(!image_size_dialog.exec()==QDialog::Accepted){
                    return;
                }
                QDir makedir(makedir_txt);
                if(!makedir.exists(doc_number_txt)){
                   makedir.mkdir(doc_number_txt);
                }
                QString output_file = outputfilename(outputpath(source_text));
                QString des_file = QString("%1/%2/%3").arg(makedir_txt).arg(doc_number_txt).arg(output_file);
                source_file.copy(des_file);

                QTextCursor cursor = this->textCursor();
                QTextImageFormat imageformat;
                imageformat.setName(QString("%1%2").arg("file:///").arg(des_file));
                imageformat.setHeight(image_size_dialog.getHeight());
                imageformat.setWidth(image_size_dialog.getWidth());
                cursor.insertImage(imageformat);
                image_list.append(des_file);
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
            int size = currentCharFormat().font().pointSize();
            QString font_family =currentCharFormat().font().family();
            eismain->ui->fontsize->setValue(size);
            eismain->ui->font_type->setCurrentText(font_family);
        }else if (objname == "EIS_listview_item"){
            EIS_listview_item *eis_listview_item = (EIS_listview_item *)parent_widget;
            int size = currentCharFormat().font().pointSize();
            QString font_family =currentCharFormat().font().family();
            eis_listview_item->ui->fontsize->setValue(size);
            eis_listview_item->ui->font_type->setCurrentText(font_family);
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
 * @brief BTextEdit::outputpath
 * 전체 경로
 * @param source
 * @return
 *
 */
QString BTextEdit::outputpath(QString source)
{
    QString result;
    result = source.split("file:///").at(1);
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


