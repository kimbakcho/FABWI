#ifndef EIS_LIST_VIEW_H
#define EIS_LIST_VIEW_H

#include <QWidget>
#include <QStringList>
#include <eis_listview_item.h>
#include <QLabel>

namespace Ui {
class Eis_list_view;
}

class Eis_list_view : public QWidget
{
    Q_OBJECT

public:
    explicit Eis_list_view(QWidget *parent = 0);
    Eis_list_view(QStringList check_list,QWidget *parent = 0);
    QVector<EIS_listview_item *>item_list;
    void resizeEvent(QResizeEvent *event);
    void modify_button_show(bool choice);
    void attach_button_show(bool choice);
    void worktime_readonly_mode(bool choice);
    ~Eis_list_view();

private:
    void closeEvent(QCloseEvent *event);
    Ui::Eis_list_view *ui;
};

#endif // EIS_LIST_VIEW_H
