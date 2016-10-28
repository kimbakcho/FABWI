#ifndef SERVER_SETTING_H
#define SERVER_SETTING_H

#include <QDialog>
#include <QDebug>
#include <QSettings>

namespace Ui {
class Server_setting;
}

class Server_setting : public QDialog
{
    Q_OBJECT

public:
    explicit Server_setting(QWidget *parent = 0);
    ~Server_setting();
private:
    void closeEvent(QCloseEvent *);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();



private:
    Ui::Server_setting *ui;
};

#endif // SERVER_SETTING_H
