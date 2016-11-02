#include "eis_big_view.h"
#include "ui_eis_big_view.h"

EIS_big_view::EIS_big_view(QString title, BTextEdit *textedit, QGridLayout *source, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EIS_big_view)
{
    ui->setupUi(this);
    this->title = title;
    this->textedit  = textedit;
    this->source = source;
    ui->center_layout->addWidget(textedit);
    this->setWindowTitle(title);

    QString font_family = ui->fontComboBox->fontInfo().family();
    int font_size = ui->font_size->value();
    mainfont.setFamily(font_family);
    mainfont.setPointSize(font_size);

    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    textedit->setCurrentCharFormat(charfotmet);

    connect(textedit,SIGNAL(cursorPositionChanged()),
                this,SLOT(b_text_cursor_change()));
}


EIS_big_view::~EIS_big_view()
{
    delete ui;
}

void EIS_big_view::closeEvent(QCloseEvent *event)
{
    source->addWidget(textedit);
    this->deleteLater();
}

void EIS_big_view::on_accpet_btn_clicked()
{
    this->close();
}

void EIS_big_view::b_text_cursor_change()
{
    if(textedit->textCursor().selection().toPlainText()==""){
        int size = textedit->currentCharFormat().font().pointSize();
        QString font_family =textedit->currentCharFormat().font().family();
        ui->font_size->setValue(size);
        ui->fontComboBox->setCurrentText(font_family);
    }
}

void EIS_big_view::on_font_size_editingFinished()
{
    mainfont.setPointSize(ui->font_size->value());
    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    textedit->setCurrentCharFormat(charfotmet);
}

void EIS_big_view::on_fontComboBox_currentIndexChanged(const QString &arg1)
{
    mainfont.setFamily(arg1);
    QTextCharFormat charfotmet;
    charfotmet.setFont(mainfont,QTextCharFormat::FontPropertiesAll);
    textedit->setCurrentCharFormat(charfotmet);
}
