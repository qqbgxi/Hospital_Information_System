#include "dlg_patientprescriptions.h"
#include "ui_dlg_patientprescriptions.h"

dlg_patientprescriptions::dlg_patientprescriptions(DBConnector *dbConnector, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::dlg_patientprescriptions)
    , dbConnector(dbConnector)
{
    ui->setupUi(this);
    loadPatientPrescriptions();
}

dlg_patientprescriptions::~dlg_patientprescriptions()
{
    delete ui;
}

void dlg_patientprescriptions::loadPatientPrescriptions()
{
    QVector<QStringList> data = dbConnector->getPatientPrescriptionView();
    ui->tableWidget->setRowCount(data.size());
    ui->tableWidget->setColumnCount(5);

    QStringList headers = {"患者ID", "患者姓名", "性别", "处方数量", "总消费金额(元)"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j < data[i].size(); ++j) {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(data[i][j]));
        }
    }
}
