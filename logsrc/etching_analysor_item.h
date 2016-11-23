#ifndef ETCHING_ANALYSOR_ITEM_H
#define ETCHING_ANALYSOR_ITEM_H

#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QLineSeries>
#include <QColorDialog>
#include <QLineSeries>
namespace Ui {
class etching_analysor_item;
}
QT_CHARTS_USE_NAMESPACE
class line_item{
public :
    QString item_name;
    QString item_type;
    QLineSeries line;

};
class etching_analysor_item : public QWidget
{
    Q_OBJECT

public:
    explicit etching_analysor_item(QWidget *parent = 0);
    QString file_path;
    void set_check_box_text(QString str);

    QCheckBox *check_box;
    QLabel *LA_file_name;
    QLabel *LA_information;

    QStringList dataline;
    QStringList data_type_list;
    QStringList item_list;
    int data_start_row;
    QColor lineColor;
    QString waferid;

    QVector<line_item *> lines;

    ~etching_analysor_item();

    QString getFile_path() const;
    void setFile_path(const QString &value);

    QCheckBox *getCheck_box() const;
    void setCheck_box(QCheckBox *value);

    QLabel *getLA_file_path() const;
    void setLA_file_path(QLabel *value);

    QLabel *getLA_information() const;
    void setLA_information(QLabel *value);

    QColor getLineColor() const;
    void setLineColor(const QColor &value);

signals:

    void change_linevisible(bool,QColor,QString);

private slots:
    void on_checkBox_toggled(bool checked);

private:
    Ui::etching_analysor_item *ui;
};

#endif // ETCHING_ANALYSOR_ITEM_H
