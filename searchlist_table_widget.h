#ifndef SEARCHLIST_TABLE_WIDGET_H
#define SEARCHLIST_TABLE_WIDGET_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QLabel>
class searchlist_table_widget : public QTableWidget
{
    Q_OBJECT
public:
    searchlist_table_widget(QObject *parent = 0);
private:
   void keyPressEvent(QKeyEvent *event);
};

#endif // SEARCHLIST_TABLE_WIDGET_H
