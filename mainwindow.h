#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <idix_gui.h>
#include <e2r_every_report.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    QString testhagl;
    ~MainWindow();

private slots:
    void on_action_everyday_report_triggered();

    void on_actionEISystem_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
