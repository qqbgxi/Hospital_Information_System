#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "dlg_addpatient.h"
#include "dlg_adddoctor.h"
#include "dlg_addmedicine.h"
#include "dlg_prescriptiondetail.h"
#include "dlg_addprescription.h"
#include "dlg_patientprescriptions.h"
#include "dlg_addregistration.h"
#include <QTreeWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->treeWidget, &QTreeWidget::itemClicked,
            this, &MainWindow::on_treeWidget_itemClicked);

    connect(ui->btn_exit, &QPushButton::clicked,
            this, &MainWindow::on_btn_exit_clicked);
    // 初始化数据库连接
    dbConnector = new DBConnector(this);
    if(!dbConnector->connectToDatabase("localhost", "hospital", "root", "wyq050828")) {
        QMessageBox::critical(this, "错误", "数据库连接失败!");
        return;
    }

    // 初始化服务
    patientService = new patient(dbConnector->getDatabase(), this);
    doctorService = new doctor(dbConnector->getDatabase(), this);
    registrationService = new registration(dbConnector->getDatabase(), this);
    medicineService = new medicine(dbConnector->getDatabase(), this);
    prescriptionService = new prescription(dbConnector->getDatabase(), this);

    // 刷新列表
    refreshPatientList();
    refreshDoctorList();
    refreshRegistrationList();
    refreshMedicineList();
    refreshPrescriptionList();

    // 连接信号槽
    connect(ui->btn_add, &QPushButton::clicked, this, &MainWindow::onPatientAdd);
    connect(ui->btn_del, &QPushButton::clicked, this, &MainWindow::onPatientDelete);
    connect(ui->btn_update, &QPushButton::clicked, this, &MainWindow::onPatientUpdate);

    connect(ui->btn_add2, &QPushButton::clicked, this, &MainWindow::onDoctorAdd);

    connect(ui->btn_add3, &QPushButton::clicked, this, &MainWindow::onRegistrationAdd);

    connect(ui->btn_add4, &QPushButton::clicked, this, &MainWindow::onMedicineAdd);

    connect(ui->prescriptionTable, &QTableWidget::itemDoubleClicked, this, &MainWindow::onPrescriptionDoubleClicked);
    connect(ui->btn_del5, &QPushButton::clicked, this, &MainWindow::onPrescriptionDelete);
    connect(ui->btn_view, &QPushButton::clicked, this, &MainWindow::showPatientPrescriptions);

    QStringList l;
    l<<"人员管理";
    QTreeWidgetItem *pf1 = new QTreeWidgetItem(ui->treeWidget,l);
    ui->treeWidget->addTopLevelItem(pf1);
    l.clear();
    l<<"患者管理";
    QTreeWidgetItem *p1=new QTreeWidgetItem(pf1,l);
    l.clear();
    l<<"医生管理";
    QTreeWidgetItem *p2=new QTreeWidgetItem(pf1,l);
    pf1->addChild(p1);
    pf1->addChild(p2);

    ui->treeWidget->expandAll();
    ui->stackedWidget->setCurrentIndex(0);

    QStringList p;
    p<<"预约挂号管理";
    QTreeWidgetItem *pf2 = new QTreeWidgetItem(ui->treeWidget,p);
    ui->treeWidget->addTopLevelItem(pf2);

    QStringList o;
    o<<"药品管理";
    QTreeWidgetItem *pf3 = new QTreeWidgetItem(ui->treeWidget,o);
    ui->treeWidget->addTopLevelItem(pf3);

    QStringList r;
    r << "处方管理";
    QTreeWidgetItem *pf4 = new QTreeWidgetItem(ui->treeWidget, r);
    ui->treeWidget->addTopLevelItem(pf4);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPatientDelete()
{
    QModelIndexList selected = ui->patientTable->selectionModel()->selectedRows();
    if(selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的患者!");
        return;
    }

    QString patientId = ui->patientTable->item(selected.first().row(), 0)->text();

    if(QMessageBox::question(this, "确认", "确定删除该患者及其所有相关记录吗?") == QMessageBox::Yes) {
        if(patientService->deletePatientWithRecords(patientId)) {
            QMessageBox::information(this, "成功", "删除成功!");
            refreshPatientList();
            refreshRegistrationList();
            refreshPrescriptionList();
        } else {
            QMessageBox::critical(this, "错误", "删除失败!");
        }
    }

}

void MainWindow::onPatientUpdate()
{

    QModelIndexList selected = ui->patientTable->selectionModel()->selectedRows();
    if(selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要修改的患者!");
        return;
    }

    int i = selected.first().row();

    PatientInfo info;
    info.p_id = ui->patientTable->item(i,0)->text();
    info.p_name = ui->patientTable->item(i,1)->text();
    info.gender = ui->patientTable->item(i,2)->text();
    QString birthday = ui->patientTable->item(i,3)->text();
    info.birth = QDate::fromString(birthday, "yyyy-MM-dd");
    info.phone = ui->patientTable->item(i,4)->text();
    info.address = ui->patientTable->item(i,5)->text();

    dlg_addpatient *dlg = new dlg_addpatient(this);

    connect(dlg, &dlg_addpatient::patientUpdated, this, [this](PatientInfo info) {
        if(patientService->updatePatient(info.p_id, info.p_name, info.gender,
                                       info.birth, info.phone, info.address)) {
            refreshPatientList();
        } else {
            QMessageBox::critical(this, "错误", "添加患者失败!");
        }
    });
    dlg->setType(false, info);
    dlg->exec();
    dlg->deleteLater();

}

void MainWindow::refreshPatientList()
{
    QVector<QStringList> patients = patientService->getAllPatients();
    ui->patientTable->setRowCount(patients.size());
    ui->patientTable->setColumnCount(6);
    //只选中行
    ui->patientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->patientTable->verticalHeader()->setVisible(false);
    ui->patientTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    border-bottom: 2px solid #555; /* 底部2像素实线 */"
        "    padding: 4px;"
        "    background-color: #f0f0f0; /* 浅灰色背景 */"
        "}");
    QStringList headers;
    headers << "患者ID" << "姓名" << "性别" << "出生日期" << "电话" << "地址";
    ui->patientTable->setHorizontalHeaderLabels(headers);

    for(int i = 0; i < patients.size(); ++i) {
        for(int j = 0; j < patients[i].size(); ++j) {
            ui->patientTable->setItem(i, j, new QTableWidgetItem(patients[i][j]));
        }
    }
}

void MainWindow::onDoctorAdd()
{
    dlg_adddoctor *dlg = new dlg_adddoctor(this);
    connect(dlg, &dlg_adddoctor::doctorAdded, this, [this](DoctorInfo info) {
        if(doctorService->addDoctor(info.d_id, info.d_name, info.gender,
                                       info.title, info.specialty, info.dept_id)) {
            refreshDoctorList();
        } else {
            QMessageBox::critical(this, "错误", "添加医生失败!");
        }
    });

    DoctorInfo newInfo;

    dlg->setType(true, newInfo);
    dlg->exec();
    dlg->deleteLater();
}

void MainWindow::onRegistrationAdd()
{
    dlg_addregistration *dlg = new dlg_addregistration(this);
    connect(dlg, &dlg_addregistration::registrationAdded, this, [this](RegistrationInfo info) {
        if(registrationService->addRegistration(info.p_id, info.d_id, info.date)) {
            refreshRegistrationList();
        } else {
            QMessageBox::critical(this, "错误", "添加预约挂号记录失败!");
        }
    });

    RegistrationInfo newInfo;

    dlg->setType(true, newInfo);
    dlg->exec();
    dlg->deleteLater();
}

void MainWindow::refreshDoctorList()
{
    QVector<QStringList> doctors = doctorService->getAllDoctors();
    ui->doctorTable->setRowCount(doctors.size());
    ui->doctorTable->setColumnCount(6);
    //只选中行
    ui->doctorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->doctorTable->verticalHeader()->setVisible(false);
    ui->doctorTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    border-bottom: 2px solid #555; /* 底部2像素实线 */"
        "    padding: 4px;"
        "    background-color: #f0f0f0; /* 浅灰色背景 */"
        "}");
    QStringList headers;
    headers << "医生ID" << "姓名" << "性别" << "职称" << "专业领域" << "科室ID";
    ui->doctorTable->setHorizontalHeaderLabels(headers);

    for(int i = 0; i < doctors.size(); ++i) {
        for(int j = 0; j < doctors[i].size(); ++j) {
            ui->doctorTable->setItem(i, j, new QTableWidgetItem(doctors[i][j]));
        }
    }
}

//切换患者管理界面和医生管理界面
void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    if(item->text(column) == "患者管理")
    {
        ui->stackedWidget->setCurrentWidget(ui->page);
    }
    else if(item->text(column) == "医生管理")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_2);
    }
    else if(item->text(column) == "预约挂号管理")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_3);
    }
    else if(item->text(column) =="药品管理")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_4);
    }
    else if(item->text(column) =="处方管理")
    {
        ui->stackedWidget->setCurrentWidget(ui->page_5);
    }
}

void MainWindow::onPatientAdd()
{
    dlg_addpatient *dlg = new dlg_addpatient(this);
    connect(dlg, &dlg_addpatient::patientAdded, this, [this](PatientInfo info) {
        if(patientService->addPatient(info.p_id, info.p_name, info.gender,
                                       info.birth, info.phone, info.address)) {
            refreshPatientList();
        } else {
            QMessageBox::critical(this, "错误", "添加患者失败!");
        }
    });

    // 设置对话框为添加模式
    PatientInfo newInfo;
    // 可以在这里生成新的患者ID或留空让用户输入
    newInfo.p_id = generateNewPatientId(); // 需要实现这个函数
    dlg->setType(true, newInfo);
    dlg->exec();
    dlg->deleteLater();
}

QString MainWindow::generateNewPatientId()
{
    // 简单实现 - 实际应用中应该从数据库获取最大ID+1或使用UUID等
    static int counter = 0;
    return QString("P%1").arg(QDateTime::currentDateTime().toSecsSinceEpoch() + counter++);
}

void MainWindow::refreshRegistrationList()
{
    QVector<QStringList> Registrations = registrationService->getAllRegistrations();
    ui->registerTable->setRowCount(Registrations.size());
    ui->registerTable->setColumnCount(3);
    //只选中行
    ui->registerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->registerTable->verticalHeader()->setVisible(false);
    ui->registerTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    border-bottom: 2px solid #555; /* 底部2像素实线 */"
        "    padding: 4px;"
        "    background-color: #f0f0f0; /* 浅灰色背景 */"
        "}");
    QStringList headers;
    headers << "患者ID" << "医生ID" << "时间";
    ui->registerTable->setHorizontalHeaderLabels(headers);

    for(int i = 0; i < Registrations.size(); ++i) {
        for(int j = 0; j < Registrations[i].size(); ++j) {
            ui->registerTable->setItem(i, j, new QTableWidgetItem(Registrations[i][j]));
        }
    }
}

void MainWindow::onMedicineAdd()
{
    Dlg_AddMedicine *dlg = new Dlg_AddMedicine(this);
    connect(dlg, &Dlg_AddMedicine::medicineAdded, this, [this](MedicineInfo info) {
        if(medicineService->addMedicine(info.m_id, info.m_name, info.unit_price,
                                       info.stock, info.productionDate, info.expiryDate))
        {
            refreshMedicineList();
        } else {
            QMessageBox::critical(this, "错误", "添加药品失败!");
        }
    });

    // 设置对话框为添加模式
    MedicineInfo newInfo;
    dlg->setType(true, newInfo);
    dlg->exec();
    dlg->deleteLater();
}

void MainWindow::refreshMedicineList()
{
    QVector<QStringList> medicines = medicineService->getAllMedicines();
    ui->medicineTable->setRowCount(medicines.size());
    ui->medicineTable->setColumnCount(6);
    //只选中行
    ui->medicineTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->medicineTable->verticalHeader()->setVisible(false);
    ui->medicineTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    border-bottom: 2px solid #555; /* 底部2像素实线 */"
        "    padding: 4px;"
        "    background-color: #f0f0f0; /* 浅灰色背景 */"
        "}");
    QStringList headers;
    headers << "药品ID" << "名称" << "单价" << "库存" << "生产日期" << "有效期限";
    ui->medicineTable->setHorizontalHeaderLabels(headers);

    for(int i = 0; i < medicines.size(); ++i) {
        for(int j = 0; j < medicines[i].size(); ++j) {
            ui->medicineTable->setItem(i, j, new QTableWidgetItem(medicines[i][j]));
        }
    }
}

void MainWindow::refreshPrescriptionList()
{
    QVector<QStringList> prescriptions = prescriptionService->getAllPrescriptions();
    ui->prescriptionTable->setRowCount(prescriptions.size());
    ui->prescriptionTable->setColumnCount(5);
    ui->prescriptionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->prescriptionTable->verticalHeader()->setVisible(false);
    ui->prescriptionTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    border-bottom: 2px solid #555;"
        "    padding: 4px;"
        "    background-color: #f0f0f0;"
        "}");
    QStringList headers;
    headers << "处方ID" << "患者ID" << "医生ID" << "开具日期" << "总金额";
    ui->prescriptionTable->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < prescriptions.size(); ++i) {
        for (int j = 0; j < prescriptions[i].size(); ++j) {
            ui->prescriptionTable->setItem(i, j, new QTableWidgetItem(prescriptions[i][j]));
        }
    }
}

void MainWindow::onPrescriptionDoubleClicked(QTableWidgetItem *item)
{
    // 获取双击行的处方ID
    int row = item->row();
    QString prescriptionId = ui->prescriptionTable->item(row, 0)->text();  // 假设处方ID在第一列

    // 查询处方细节
    QVector<QStringList> prescriptionDetails = prescriptionService->getPrescriptionDetails(prescriptionId);

    // 显示处方细节对话框
    dlg_prescriptiondetail *dlg = new dlg_prescriptiondetail(this);
    dlg->setPrescriptionDetails(prescriptionDetails);
    dlg->exec();
    dlg->deleteLater();
}

void MainWindow::onPrescriptionDelete()
{
    QModelIndexList selected = ui->prescriptionTable->selectionModel()->selectedRows();
    if(selected.isEmpty()) {
        QMessageBox::warning(this, "警告", "请选择要删除的处方!");
        return;
    }

    QString prescription = ui->prescriptionTable->item(selected.first().row(), 0)->text();
    if(QMessageBox::question(this, "确认", "确定删除该处方吗?") == QMessageBox::Yes) {
        if(prescriptionService->deletePrescriptionWithRecords(prescription)) {
            QMessageBox::information(this, "成功", "删除成功!");
            refreshPrescriptionList();
        } else {
            QMessageBox::critical(this, "错误", "删除失败!");
        }
    }
}

void MainWindow::on_btn_exit_clicked()
{
    exit(0);
}


void MainWindow::on_btn_add5_clicked()
{
    dlg_addprescription *dlg = new dlg_addprescription(this);
    dlg->exec();
    dlg->deleteLater();
}

void MainWindow::showPatientPrescriptions()
{
    dlg_patientprescriptions *dlg = new dlg_patientprescriptions(dbConnector, this);
    dlg->exec();
    dlg->deleteLater();
}

void MainWindow::on_btn_search5_clicked()
{
    QString strFilter = ui->le_search5->text();
    if(strFilter.isEmpty())
    {
        refreshPrescriptionList();
        return;
    }

    ui->prescriptionTable->clear();
    ui->prescriptionTable->setColumnCount(5);
    ui->prescriptionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->prescriptionTable->verticalHeader()->setVisible(false);
    ui->prescriptionTable->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "    border-bottom: 2px solid #555;"
        "    padding: 4px;"
        "    background-color: #f0f0f0;"
        "}");
    QStringList headers;
    headers << "处方ID" << "患者ID" << "医生ID" << "开具日期" << "总金额";
    ui->prescriptionTable->setHorizontalHeaderLabels(headers);

    QVector<QStringList> viewData = dbConnector->getPatientPrescriptions(strFilter);
    ui->prescriptionTable->setRowCount(viewData.size());
    for (int i = 0; i < viewData.size(); ++i) {
        for (int j = 0; j < viewData[i].size(); ++j) {
            ui->prescriptionTable->setItem(i, j, new QTableWidgetItem(viewData[i][j]));
        }
    }
}

