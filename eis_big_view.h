#ifndef EIS_BIG_VIEW_H
#define EIS_BIG_VIEW_H

#include <QWidget>
#include <btextedit.h>
#include <QGridLayout>
#include <QFontComboBox>
#include <QSpinBox>
#include <QPushButton>
#include "btextedit.h"
#include <QLabel>
namespace Ui {
class EIS_big_view;
}

class EIS_big_view : public QWidget
{
    Q_OBJECT

public:
    explicit EIS_big_view(QString title, BTextEdit *textedit, QFontComboBox *fontbox,
                          QSpinBox *textspinbox, QPushButton *bold_btn, QPushButton *underline_btn,
                          QLabel *now_color,QPushButton *color_dialog,
                          QGridLayout *source, QHBoxLayout *font_source,QWidget *parent = 0);
    BTextEdit *textedit;
    QFontComboBox *fontbox;
    QSpinBox *textspinbox;
    QPushButton *bold_btn;
    QPushButton *underline_btn;
    QLabel *now_color;
    QPushButton *color_dialog;
    QString title;
    QGridLayout *source;
    QHBoxLayout *font_source;
    QFont mainfont;
    ~EIS_big_view();

private slots:

    void on_accpet_btn_clicked();


private:
    Ui::EIS_big_view *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // EIS_BIG_VIEW_H
