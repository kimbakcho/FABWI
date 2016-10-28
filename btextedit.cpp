#include "btextedit.h"

BTextEdit::BTextEdit(QWidget *parent) : QTextBrowser(parent)
{
    this->setTextInteractionFlags(Qt::TextEditable);
}
BTextEdit::BTextEdit(int *doc_number, QWidget *parent):QTextBrowser(parent)
{
    this->setTextInteractionFlags(Qt::TextEditable);
    this->doc_number = doc_number;
}


void BTextEdit::insertFromMimeData( const QMimeData *source ){

          QString source_text = source->text();

          QString doc_number_txt = QString("%1").arg(*doc_number);
          if(ispicture(source_text)){
                QFile source_file(outputpath(source_text),this);
                QString makedir_txt = qApp->applicationDirPath()+"/temp/EIS";
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
                cursor.insertImage(imageformat);
          }
}
void BTextEdit::replyFinished(QNetworkReply* reply){
    qDebug()<<"finish";
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

QString BTextEdit::outputpath(QString source)
{
    QString result;
    result = source.split("file:///").at(1);
    return result;
}

QString BTextEdit::outputfilename(QString source)
{
    QString result;
    result = source.split("/").last();
    QString nowdatetime = QDateTime::currentDateTime().toString("yyyy_MM_dd_HH_mm_ss_");
    result = nowdatetime + result;
    return result;
}

