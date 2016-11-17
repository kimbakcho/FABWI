#include "eis_delete_dialog.h"
#include "ui_eis_delete_dialog.h"

EIS_delete_dialog::EIS_delete_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EIS_delete_dialog)
{
    ui->setupUi(this);
    db = QSqlDatabase::database("EISDB");
    password_alright = false;

}

bool EIS_delete_dialog::getPassword_alright() const
{
    return password_alright;
}

EIS_delete_dialog::~EIS_delete_dialog()
{
    delete ui;
}

void EIS_delete_dialog::on_buttonBox_accepted()
{
    QSqlQuery query(db);
    query.exec("select delete_password from EIS_management");
    query.next();
    if(ui->LE_password->text() == query.value("delete_password").toString()){
        password_alright = true;
        close();
    }else {
        password_alright = false;
        ui->staute_label->setText("wrong password");
    }
}
