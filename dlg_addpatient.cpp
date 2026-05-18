#include "dlg_addpatient.h"
#include "ui_dlg_addpatient.h"
#include"dbconnector.h"
#include"QMessageBox"

dlg_addpatient::dlg_addpatient(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dlg_AddPatient)
{
    ui->setupUi(this);
}

dlg_addpatient::~dlg_addpatient()
{
    delete ui;
}

void dlg_addpatient::setType(bool isAdd,PatientInfo info)
{
    m_isAdd = isAdd;
    m_info = info;
    ui->le_id->setText(info.p_id);
    ui->le_name->setText(info.p_name);
    ui->le_birth->setText(info.birth.toString("yyyy-MM-dd"));
    ui->le_phone->setText(info.phone);
    ui->le_address->setText(info.address);

    if (!isAdd) { // 如果是更新模式
        ui->le_id->setReadOnly(true); // 禁止修改患者ID
        ui->btn_save->setText("更新");
    } else {
        ui->le_id->setReadOnly(false);
        ui->btn_save->setText("保存");
    }
}

void dlg_addpatient::on_btn_save_clicked()
{
    PatientInfo info;
    info.p_id = ui->le_id->text();
    info.p_name = ui->le_name->text();
    info.gender = ui->le_gender->text();
    info.birth = QDate::fromString(ui->le_birth->text(),"yyyy-MM-dd");
    info.phone = ui->le_phone->text();
    info.address = ui->le_address->text();

    if(m_isAdd)
    {
        emit patientAdded(info);
    }
    else
    {
        emit patientUpdated(info);
    }
    QMessageBox::information(nullptr,"信息","存储成功");
    this->accept();
}


void dlg_addpatient::on_btn_cancel_clicked()
{
    this->reject();
}

