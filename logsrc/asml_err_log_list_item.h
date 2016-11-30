#ifndef ASML_ERR_LOG_LIST_ITEM_H
#define ASML_ERR_LOG_LIST_ITEM_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QtCharts/QChart>
#include <QLineSeries>
#include <QBarset>
#include <QDebug>
QT_CHARTS_USE_NAMESPACE

namespace Ui {
class ASML_err_log_list_item;
}

class ASML_err_log_list_item : public QWidget
{
    Q_OBJECT

public:
    explicit ASML_err_log_list_item(QString bar_name,QWidget *parent = 0);
    QLabel *LA_data_name;
    QLabel *LA_data_count;
    QLabel *LA_data_color;
    QCheckBox *Cb_check_box;
    QBarSet *bar_data;
    QLineSeries *linedata;
    QString source_data;
    QString explan;
    void set_dataname(QString str);
    void set_count_data(QString str);
    ~ASML_err_log_list_item();

    QLabel *getLA_data_name() const;
    void setLA_data_name(QLabel *value);

    QLabel *getLA_data_count() const;
    void setLA_data_count(QLabel *value);

    QLabel *getLA_data_color() const;
    void setLA_data_color(QLabel *value);

    QCheckBox *getCb_check_box() const;
    void setCb_check_box(QCheckBox *value);

    QBarSet *getBar_data() const;
    void setBar_data(QBarSet *value);

    QLineSeries *getLinedata() const;
    void setLinedata(QLineSeries *value);

    QString getSource_data() const;
    void setSource_data(const QString &value);

    QString getExplan() const;
    void setExplan(const QString &value);

signals :
    void sig_hover_change(QString,QString);
private slots:

    void slot_barset_hover(bool status,int index);
    void on_checkBox_toggled(bool checked);

private:
    Ui::ASML_err_log_list_item *ui;
};

#endif // ASML_ERR_LOG_LIST_ITEM_H
