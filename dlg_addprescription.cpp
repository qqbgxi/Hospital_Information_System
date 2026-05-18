#include "dlg_addprescription.h"
#include "ui_dlg_addprescription.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include "dbconnector.h"
#include "prescription.h"
#include "mainwindow.h"

dlg_addprescription::dlg_addprescription(MainWindow *mainWindow, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dlg_addprescription)
    , mainWindow(mainWindow)
{
    ui->setupUi(this);
    dbConnector = new DBConnector(this);
    prescriptionService = new prescription(dbConnector->getDatabase(), this);

    ui->detailTable->setColumnCount(2);
    QStringList headers;
    headers << "药品ID" << "用法用量";
    ui->detailTable->setHorizontalHeaderLabels(headers);
}

dlg_addprescription::~dlg_addprescription()
{
    delete ui;
}


QString dlg_addprescription::getPatientId() const
{
    return ui->patientIdEdit->text();
}

QString dlg_addprescription::getDoctorId() const
{
    return ui->doctorIdEdit->text();
}

void dlg_addprescription::on_addDetailButton_clicked()
{
    QString medicineId = ui->medicineIdEdit->text();
    QString usage = ui->usageEdit->text();
    if (medicineId.isEmpty() || usage.isEmpty()) {
        QMessageBox::warning(this, "警告", "药品ID和用法用量不能为空！");
        return;
    }
    int row = ui->detailTable->rowCount();
    qDebug() << "Inserting row:" << row;
    ui->detailTable->insertRow(row);
    ui->detailTable->setItem(row, 0, new QTableWidgetItem(medicineId));
    ui->detailTable->setItem(row, 1, new QTableWidgetItem(usage));
}

void dlg_addprescription::on_saveButton_clicked()
{
    QString prescriptionId = ui->prescriptionIdEdit->text();
    QString patientId = getPatientId();
    QString doctorId = getDoctorId();
    QDate issueDate = ui->dateEdit->date();
    // 收集所有药品和用法
    QStringList medicineIds;
    QStringList usages;
    for (int i = 0; i < ui->detailTable->rowCount(); ++i) {
        medicineIds << ui->detailTable->item(i, 0)->text();
        usages << ui->detailTable->item(i, 1)->text();
    }
    if (medicineIds.isEmpty()) {
        QMessageBox::warning(this, "警告", "请至少添加一个药品！");
        return;
    }
    QString result;
    if(prescriptionService->addPrescriptionWithMedicineCheck(prescriptionId,patientId, doctorId, issueDate,
                                                         medicineIds, usages, result))
    {
        QMessageBox::information(this, "成功", "处方信息保存成功！");
        mainWindow->refreshPrescriptionList();
        mainWindow->refreshMedicineList();
    }else{
        QMessageBox::critical(this, "错误", result);
    }
    this->accept();
}

void dlg_addprescription::on_btn_del_clicked()
{
    // 获取当前选中的行
    QModelIndexList selected = ui->detailTable->selectionModel()->selectedRows();

    if(selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的行！");
        return;
    }

    foreach(QModelIndex index, selected) {
        ui->detailTable->removeRow(index.row());
    }
}

