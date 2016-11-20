#include "etching_chart_bigview.h"
#include "ui_etching_chart_bigview.h"
#include "etching_base_chart_view.h"
#include "ui_etching_base_chart_view.h"
etching_chart_bigview::etching_chart_bigview(QObject *parent_view) :

    ui(new Ui::etching_chart_bigview)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    this->parent_view = parent_view;
    Etching_base_chart_view *base_parent_view  = (Etching_base_chart_view *)parent_view;

    ui->layout_1->insertWidget(0,base_parent_view->ui->LA_add_spec);
    ui->layout_1->insertWidget(1,base_parent_view->ui->newline_spec);
    ui->layout_1->insertWidget(2,base_parent_view->ui->LA_add_spec_value);
    ui->layout_1->insertWidget(3,base_parent_view->ui->newline_value);
    ui->layout_1->insertWidget(4,base_parent_view->ui->line_colorview);
    ui->layout_1->insertWidget(5,base_parent_view->ui->select_color_btn);
    ui->layout_1->insertWidget(6,base_parent_view->ui->line_add_btn);

    ui->layout_2->insertWidget(0,base_parent_view->ui->LA_item);
    ui->layout_2->insertWidget(1,base_parent_view->ui->cb_item);
    ui->layout_2->insertWidget(2,base_parent_view->ui->LA_type);
    ui->layout_2->insertWidget(3,base_parent_view->ui->LA_type_2);
    ui->layout_2->insertWidget(4,base_parent_view->ui->zoomreset_btn);
    ui->layout_2->insertWidget(5,base_parent_view->ui->view_linelabel_btn);

    ui->layout_3->insertWidget(0,base_parent_view->ui->LA_X_value);
    ui->layout_3->insertWidget(1,base_parent_view->ui->X_value);
    ui->layout_3->insertWidget(2,base_parent_view->ui->LA_Y_value);
    ui->layout_3->insertWidget(3,base_parent_view->ui->Y_value);

    ui->layout_3->insertWidget(0,base_parent_view->ui->LA_X_value);
    ui->layout_3->insertWidget(1,base_parent_view->ui->X_value);
    ui->layout_3->insertWidget(2,base_parent_view->ui->LA_Y_value);
    ui->layout_3->insertWidget(3,base_parent_view->ui->Y_value);

    ui->chart_layout->addWidget(base_parent_view->chartview,0,0);
}

etching_chart_bigview::~etching_chart_bigview()
{
    delete ui;
}

void etching_chart_bigview::closeEvent(QCloseEvent *event)
{
    Etching_base_chart_view *base_parent_view  = (Etching_base_chart_view *)parent_view;
    base_parent_view->ui->layout_1->insertWidget(0,base_parent_view->ui->LA_add_spec);
    base_parent_view->ui->layout_1->insertWidget(1,base_parent_view->ui->newline_spec);
    base_parent_view->ui->layout_1->insertWidget(2,base_parent_view->ui->LA_add_spec_value);
    base_parent_view->ui->layout_1->insertWidget(3,base_parent_view->ui->newline_value);
    base_parent_view->ui->layout_1->insertWidget(4,base_parent_view->ui->line_colorview);
    base_parent_view->ui->layout_1->insertWidget(5,base_parent_view->ui->select_color_btn);
    base_parent_view->ui->layout_1->insertWidget(6,base_parent_view->ui->line_add_btn);

    base_parent_view->ui->layout_2->insertWidget(0,base_parent_view->ui->LA_item);
    base_parent_view->ui->layout_2->insertWidget(1,base_parent_view->ui->cb_item);
    base_parent_view->ui->layout_2->insertWidget(2,base_parent_view->ui->LA_type);
    base_parent_view->ui->layout_2->insertWidget(3,base_parent_view->ui->LA_type_2);
    base_parent_view->ui->layout_2->insertWidget(4,base_parent_view->ui->zoomreset_btn);
    base_parent_view->ui->layout_2->insertWidget(5,base_parent_view->ui->view_linelabel_btn);

    base_parent_view->ui->layout_3->insertWidget(0,base_parent_view->ui->LA_X_value);
    base_parent_view->ui->layout_3->insertWidget(1,base_parent_view->ui->X_value);
    base_parent_view->ui->layout_3->insertWidget(2,base_parent_view->ui->LA_Y_value);
    base_parent_view->ui->layout_3->insertWidget(3,base_parent_view->ui->Y_value);

    base_parent_view->ui->layout_3->insertWidget(0,base_parent_view->ui->LA_X_value);
    base_parent_view->ui->layout_3->insertWidget(1,base_parent_view->ui->X_value);
    base_parent_view->ui->layout_3->insertWidget(2,base_parent_view->ui->LA_Y_value);
    base_parent_view->ui->layout_3->insertWidget(3,base_parent_view->ui->Y_value);

    base_parent_view->ui->chart_layout->addWidget(base_parent_view->chartview,0,0);
}
