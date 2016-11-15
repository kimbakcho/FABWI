#include "eis_big_view.h"
#include "ui_eis_big_view.h"

EIS_big_view::EIS_big_view(QString title, BTextEdit *textedit, QFontComboBox *fontbox,
                           QSpinBox *textspinbox, QPushButton *bold_btn, QPushButton *underline_btn,
                           QLabel *now_color, QPushButton *color_dialog,
                           QGridLayout *source, QHBoxLayout *font_source, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EIS_big_view)
{
    ui->setupUi(this);
    this->title = title;
    this->textedit  = textedit;
    this->source = source;
    this->font_source = font_source;
    this->fontbox = fontbox;
    this->textspinbox = textspinbox;
    this->bold_btn = bold_btn;
    this->underline_btn = underline_btn;
    this->font_source = font_source;
    this->now_color = now_color;
    this->color_dialog = color_dialog;
    ui->center_layout->addWidget(textedit);
    ui->font_layout->insertWidget(0,fontbox);
    ui->font_layout->insertWidget(1,textspinbox);
    ui->font_layout->insertWidget(2,bold_btn);
    ui->font_layout->insertWidget(3,underline_btn);
    ui->font_layout->insertWidget(4,now_color);
    ui->font_layout->insertWidget(5,color_dialog);
    this->setWindowTitle(title);

}


EIS_big_view::~EIS_big_view()
{
    delete ui;
}

void EIS_big_view::closeEvent(QCloseEvent *event)
{
    source->addWidget(textedit);
    font_source->insertWidget(0,fontbox);
    font_source->insertWidget(1,textspinbox);
    font_source->insertWidget(2,bold_btn);
    font_source->insertWidget(3,underline_btn);
    font_source->insertWidget(4,now_color);
    font_source->insertWidget(5,color_dialog);
    this->deleteLater();
}

void EIS_big_view::on_accpet_btn_clicked()
{
    this->close();
}


