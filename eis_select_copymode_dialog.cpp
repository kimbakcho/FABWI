#include "eis_select_copymode_dialog.h"
#include "ui_eis_select_copymode_dialog.h"

eis_select_copymode_dialog::eis_select_copymode_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::eis_select_copymode_dialog)
{
    ui->setupUi(this);
    mode =0;
}

eis_select_copymode_dialog::~eis_select_copymode_dialog()
{
    delete ui;
}

void eis_select_copymode_dialog::on_buttonBox_accepted()
{
    if(ui->rb_picture->isChecked()){
        mode = 1;
    }else if(ui->rb_text->isChecked()){
        mode = 2;
    }
}
