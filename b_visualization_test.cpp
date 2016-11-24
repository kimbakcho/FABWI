#include "b_visualization_test.h"
#include "ui_b_visualization_test.h"

b_visualization_test::b_visualization_test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::b_visualization_test)
{
    ui->setupUi(this);
}

b_visualization_test::~b_visualization_test()
{
    delete ui;
}
