#ifndef HANILE_LOG_MAIN_H
#define HANILE_LOG_MAIN_H

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <Qdir>
#include <QFile>
#include <hanile_log_item.h>

namespace Ui {
class hanile_log_main;
}

class hanile_log_main : public QWidget
{
    Q_OBJECT

public:
    explicit hanile_log_main(QWidget *parent = 0);
    QVector<hanile_log_item *> item_list;
    ~hanile_log_main();

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    Ui::hanile_log_main *ui;
};

#endif // HANILE_LOG_MAIN_H
