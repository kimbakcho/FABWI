#include "idix_gui.h"
#include "ui_idix_gui.h"

IDIX_gui::IDIX_gui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IDIX_gui)
{
    ui->setupUi(this);





}

IDIX_gui::~IDIX_gui()
{
    delete ui;
}
