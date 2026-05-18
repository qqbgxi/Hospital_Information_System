#include "dlg_addregistration.h"
#include "ui_dlg_addregistration.h"
#include"dbconnector.h"
#include"QMessageBox"

dlg_addregistration::dlg_addregistration(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dlg_addregistration)
{
    ui->setupUi(this);
}

dlg_addregistration::~dlg_addregistration()
{
    delete ui;
}

void dlg_addregistration::setType(bool isAdd,RegistrationInfo info)
{
    m_isAdd = isAdd;
    m_info = info;
    ui->le_pid->setText(info.p_id);
    ui->le_did->setText(info.d_id);
    ui->le_date->setDate(info.date);
}

void dlg_addregistration::on_btn_save_clicked()
{
    RegistrationInfo info;
    info.p_id = ui->le_pid->text();
    info.d_id = ui->le_did->text();
    info.date = ui->le_date->date();

    if(m_isAdd)
    {
        emit registrationAdded(info);
    }
    else
    {
        emit registrationUpdated(info);
    }
    QMessageBox::information(nullptr,"信息","存储成功");
    this->accept();
}


void dlg_addregistration::on_btn_cancel_clicked()
{
    this->reject();
}
