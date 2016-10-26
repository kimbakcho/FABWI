#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <eismain.h>
#include <e2r_every_report.h>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_everyday_report_triggered()
{
    E2R_every_report *report = new E2R_every_report();
    report->show();
}

void MainWindow::on_actionEISystem_triggered()
{
    EISmain *eismain = new EISmain();
    eismain->show();
}
