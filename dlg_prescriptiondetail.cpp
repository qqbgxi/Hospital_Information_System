#include "dlg_prescriptiondetail.h"
#include "ui_dlg_prescriptiondetail.h"

dlg_prescriptiondetail::dlg_prescriptiondetail(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dlg_prescriptiondetail)
{
    ui->setupUi(this);
}

dlg_prescriptiondetail::~dlg_prescriptiondetail()
{
    delete ui;
}

void dlg_prescriptiondetail::setPrescriptionDetails(const QVector<QStringList> &details)
{
    ui->tableWidget->setRowCount(details.size());
    ui->tableWidget->setColumnCount(2);
    QStringList headers;
    headers << "药品名称" << "用法用量";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < details.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(details[i][0]));  // 药品名称
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(details[i][1]));  // 用法用量
    }
}
