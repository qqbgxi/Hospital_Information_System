#include "dlg_adddoctor.h"
#include "ui_dlg_adddoctor.h"
#include"dbconnector.h"
#include"QMessageBox"

dlg_adddoctor::dlg_adddoctor(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dlg_AddDoctor)
{
    ui->setupUi(this);
}

dlg_adddoctor::~dlg_adddoctor()
{
    delete ui;
}

void dlg_adddoctor::setType(bool isAdd,DoctorInfo info)
{
    m_isAdd = isAdd;
    m_info = info;
    ui->le_id->setText(info.d_id);
    ui->le_name->setText(info.d_name);
    ui->le_title->setText(info.title);
    ui->le_specialty->setText(info.specialty);
    ui->le_dept->setText(info.dept_id);

}

void dlg_adddoctor::on_btn_save_clicked()
{
    DoctorInfo info;
    info.d_id = ui->le_id->text();
    info.d_name = ui->le_name->text();
    info.gender = ui->le_gender->text();
    info.title = ui->le_title->text();
    info.specialty = ui->le_specialty->text();
    info.dept_id = ui->le_dept->text();

    if(m_isAdd)
    {
        emit doctorAdded(info);
    }
    else
    {
        emit doctorUpdated(info);
    }
    QMessageBox::information(nullptr,"信息","存储成功");
    this->accept();
}


void dlg_adddoctor::on_btn_cancel_clicked()
{
    this->reject();
}

