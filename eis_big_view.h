#ifndef EIS_BIG_VIEW_H
#define EIS_BIG_VIEW_H

#include <QWidget>
#include <btextedit.h>
#include <QGridLayout>
#include "btextedit.h"
namespace Ui {
class EIS_big_view;
}

class EIS_big_view : public QWidget
{
    Q_OBJECT

public:
    explicit EIS_big_view(QString title,BTextEdit *textedit, QGridLayout *source, QWidget *parent = 0);
    BTextEdit *textedit;
    QString title;
    QGridLayout *source;
    QFont mainfont;
    ~EIS_big_view();

private slots:

    void on_accpet_btn_clicked();
    void b_text_cursor_change();

    void on_font_size_editingFinished();

    void on_fontComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::EIS_big_view *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // EIS_BIG_VIEW_H
