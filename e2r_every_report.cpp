#include "e2r_every_report.h"
#include "ui_e2r_every_report.h"
#include <QDebug>
E2R_every_report::E2R_every_report(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::E2R_every_report)
{
    ui->setupUi(this);
    /**
     * @brief settings
     * 기존에 설정해 두었던 경로를 읽는다.
     */
    configini_str = "everyday_report_config.ini";
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    QString export_path = settings.value("export_path").toString();
    QString ICP_Etch_rate_path = settings.value("ICP_Etch_rate_path").toString();
    QString SIN_Etch_rate_path  = settings.value("SIN_Etch_rate_path").toString();
    QString SIO2_Etch_rate_path = settings.value("SIO2_Etch_rate_path").toString();
    QString WPL_SiO2_path = settings.value("WPL_SiO2_path").toString();
    QString ICP_Ashing_rate_path = settings.value("ICP_Ashing_rate_path").toString();
    QString mesure_1_path = settings.value("mesure_1_path").toString();
    QString mesure_2_3_path = settings.value("mesure_2_3_path").toString();
    QString sungmak_day_path = settings.value("sungmak_day_path").toString();
    QString uniformity_ckeck_path = settings.value("uniformity_ckeck_path").toString();
    QString daily_gap_path = settings.value("daily_gap_path").toString();
    QString Daily_Bridge_dongjin_path = settings.value("Daily_Bridge_path").toString();
    QString Daily_Bridge_path = settings.value("Daily_Bridge_dongjin_path").toString();
    QString Daily_pad_path = settings.value("Daily_pad_path").toString();
    QString daily_pad_ashing_path = settings.value("daily_pad_ashing_path").toString();
    QString mesure_pr_cd_path = settings.value("mesure_pr_cd_path").toString();

    settings.endGroup();
    ui->export_path_edit->setText(export_path);
    ui->ICP_Etch_rate_path->setText(ICP_Etch_rate_path);
    ui->SIN_Etch_rate_path->setText(SIN_Etch_rate_path);
    ui->SIO2_Etch_rate_path->setText(SIO2_Etch_rate_path);
    ui->WPL_SiO2_path->setText(WPL_SiO2_path);
    ui->ICP_Ashing_rate_path->setText(ICP_Ashing_rate_path);
    ui->mesure_1_path->setText(mesure_1_path);
    ui->mesure_2_3_path->setText(mesure_2_3_path);
    ui->sungmak_day_path->setText(sungmak_day_path);
    ui->uniformity_ckeck_path->setText(uniformity_ckeck_path);
    ui->daily_gap_path->setText(daily_gap_path);
    ui->Daily_Bridge_path->setText(Daily_Bridge_path);
    ui->Daily_Bridge_dongjin_path->setText(Daily_Bridge_dongjin_path);
    ui->Daily_pad_path->setText(Daily_pad_path);
    ui->daily_pad_ashing_path->setText(daily_pad_ashing_path);
    ui->mesure_pr_cd_path->setText(mesure_pr_cd_path);


}

E2R_every_report::~E2R_every_report()
{
    delete ui;
}

void E2R_every_report::on_ICP_Etch_rate_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx)"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->ICP_Etch_rate_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("ICP_Etch_rate_path",path);
    settings.endGroup();


}

/**
 * @brief E2R_every_report::on_export_btn_clicked
 * 리포트 파일을 추출 하는 버튼 슬롯
 * 어플리케이션 폴더에서 every_day_report.xlsx 를 설정 경로에 복사한뒤
 * 각각의 파일을 형식에 맞게 읽어서
 * 내용만 덮어 씌어 준다.
 */
void E2R_every_report::on_export_btn_clicked()
{

      QFile file(ui->ICP_Etch_rate_path->text());
      QString icp_Etch_path;
      QString mesure_pr_cd_path;

      QString filename = ui->ICP_Etch_rate_path->text().split('/').last();
      QString copypath = ui->export_path_edit->text() +"/" +filename;
      file.copy(copypath);
      icp_Etch_path = copypath;

      file.setFileName(ui->SIN_Etch_rate_path->text());
      filename = ui->SIN_Etch_rate_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->SIO2_Etch_rate_path->text());
      filename = ui->SIO2_Etch_rate_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/" +filename;;
      qDebug()<<copypath;
      file.copy(copypath);


      file.setFileName(ui->WPL_SiO2_path->text());
      filename = ui->WPL_SiO2_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->ICP_Ashing_rate_path->text());
      filename = ui->ICP_Ashing_rate_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->mesure_1_path->text());
      filename = ui->mesure_1_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->mesure_2_3_path->text());
      filename = ui->mesure_2_3_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->sungmak_day_path->text());
      filename = ui->sungmak_day_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->uniformity_ckeck_path->text());
      filename = ui->uniformity_ckeck_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->daily_gap_path->text());
      filename = ui->daily_gap_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->Daily_Bridge_path->text());
      filename = ui->Daily_Bridge_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->Daily_Bridge_dongjin_path->text());
      filename = ui->Daily_Bridge_dongjin_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);


      file.setFileName(ui->Daily_pad_path->text());
      filename = ui->Daily_pad_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->daily_pad_ashing_path->text());
      filename = ui->daily_pad_ashing_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->mesure_pr_cd_path->text());
      filename = ui->mesure_pr_cd_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);

      file.setFileName(ui->mesure_pr_cd_path->text());
      filename = ui->mesure_pr_cd_path->text().split('/').last();
      copypath = ui->export_path_edit->text() +"/"+ filename;
      file.copy(copypath);
      mesure_pr_cd_path = copypath;

      file.setFileName(qApp->applicationDirPath()+"/chart_report_total.xlsm");
      copypath = ui->export_path_edit->text() + "/chart_report_total.xlsm";
      file.copy(copypath);

      if(!QFile::exists(icp_Etch_path)){
          QMessageBox msgbox;
          msgbox.setText("noacess");
          msgbox.exec();
      }else if(!QFile::exists(mesure_pr_cd_path)){
          QMessageBox msgbox;
          msgbox.setText("noacess");
          msgbox.exec();
      }else {
          QMessageBox msgbox;
          msgbox.setText("complete.");
          msgbox.exec();
      }
      QString openfile_name  = QString("%1%2").arg("file:///").arg(copypath);
      QDesktopServices::openUrl(openfile_name);
}
/**
 * @brief E2R_every_report::on_export_path_btn_clicked
 * 출력 경로를 설정한다.
 */
void E2R_every_report::on_export_path_btn_clicked()
{
    QFileDialog dialog(this);
    QString getsavefile;
    getsavefile = dialog.getExistingDirectory(this);
    ui->export_path_edit->setText(getsavefile);
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("export_path",getsavefile);
    settings.endGroup();
}

void E2R_every_report::on_excel_mecro_clicked()
{
    QString strProgram = qApp->applicationDirPath()+"/every_day_report.bat";
    QProcess *process = new QProcess( this );
    process->setWorkingDirectory(qApp->applicationDirPath());
    process->startDetached(strProgram);
}

void E2R_every_report::on_SIN_Etch_rate_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->SIN_Etch_rate_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("SIN_Etch_rate_path",path);
    settings.endGroup();
}

void E2R_every_report::on_SIO2_Etch_rate_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->SIO2_Etch_rate_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("SIO2_Etch_rate_path",path);
    settings.endGroup();
}

void E2R_every_report::on_WPL_SiO2_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->WPL_SiO2_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("WPL_SiO2_path",path);
    settings.endGroup();
}

void E2R_every_report::on_ICP_Ashing_rate_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->ICP_Ashing_rate_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("ICP_Ashing_rate_path",path);
    settings.endGroup();
}

void E2R_every_report::on_mesure_1_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->mesure_1_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("mesure_1_path",path);
    settings.endGroup();
}

void E2R_every_report::on_mesure_2_3_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->mesure_2_3_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("mesure_2_3_path",path);
    settings.endGroup();
}

void E2R_every_report::on_sungmak_day_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->sungmak_day_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("sungmak_day_path",path);
    settings.endGroup();
}

void E2R_every_report::on_uniformity_ckeck_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->uniformity_ckeck_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("uniformity_ckeck_path",path);
    settings.endGroup();
}


void E2R_every_report::on_daily_gap_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->daily_gap_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("daily_gap_path",path);
    settings.endGroup();
}

void E2R_every_report::on_Daily_Bridge_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->Daily_Bridge_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("Daily_Bridge_path",path);
    settings.endGroup();
}
void E2R_every_report::on_Daily_Bridge_dongjin_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->Daily_Bridge_dongjin_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("Daily_Bridge_dongjin_path",path);
    settings.endGroup();
}
void E2R_every_report::on_Daily_pad_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->Daily_pad_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("Daily_pad_path",path);
    settings.endGroup();
}

void E2R_every_report::on_daily_pad_ashing_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->daily_pad_ashing_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("daily_pad_ashing_path",path);
    settings.endGroup();
}

void E2R_every_report::on_mesure_pr_cd_btn_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("excelfile (*.xlsx *.xls)"));
    dialog.setFileMode(QFileDialog::ExistingFile);\
    dialog.exec();
    QStringList fileselect = dialog.selectedFiles();
    QString path ="";
    if(!fileselect.isEmpty()){
        path=fileselect.first();
        ui->mesure_pr_cd_path->setText(fileselect.first());
    }
    QSettings settings(configini_str,QSettings::IniFormat);
    settings.beginGroup("path");
    settings.setValue("mesure_pr_cd_path",path);
    settings.endGroup();
}


