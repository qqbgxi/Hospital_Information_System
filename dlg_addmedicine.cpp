#include "dlg_addmedicine.h"
#include "ui_dlg_addmedicine.h"
#include "dbconnector.h"
#include "QMessageBox"

Dlg_AddMedicine::Dlg_AddMedicine(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dlg_AddMedicine)
{
    ui->setupUi(this);
}

Dlg_AddMedicine::~Dlg_AddMedicine()
{
    delete ui;
}

void Dlg_AddMedicine::setType(bool isAdd,MedicineInfo info)
{
    m_isAdd = isAdd;
    m_info = info;
    ui->le_id->setText(info.m_id);
    ui->le_name->setText(info.m_name);
    ui->le_unitprice->setText(QString::number(info.unit_price, 'f', 2));
    ui->le_stock->setText(QString::number(info.stock));
    ui->le_expiryDate->setText(info.expiryDate.toString("yyyy-MM-dd"));
    ui->le_productionDate->setText(info.productionDate.toString("yyyy-MM-dd"));
}

void Dlg_AddMedicine::on_btn_save_clicked()
{
    MedicineInfo info;
    info.m_id = ui->le_id->text();
    info.m_name = ui->le_name->text();
    info.unit_price = ui->le_unitprice->text().toDouble();
    info.stock = ui->le_stock->text().toInt();
    info.expiryDate = QDate::fromString(ui->le_expiryDate->text(),"yyyy-MM-dd");
    info.productionDate = QDate::fromString(ui->le_productionDate->text(),"yyyy-MM-dd");

    if(m_isAdd)
    {
        emit medicineAdded(info);
    }
    else
    {
        emit medicineUpdated(info);
    }
    this->accept();
}


void Dlg_AddMedicine::on_btn_cancel_clicked()
{
    this->reject();
}

