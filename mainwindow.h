#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dbconnector.h"
#include "patient.h"
#include "doctor.h"
#include "registration.h"
#include "medicine.h"
#include "prescription.h"
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void onPatientAdd();
    void onPatientDelete();
    void onPatientUpdate();
    void refreshPatientList();

    void onDoctorAdd();
    void onDoctorDelete();
    void refreshDoctorList();

    QString generateNewPatientId();
    void on_btn_exit_clicked();

    void refreshRegistrationList();

    void onMedicineAdd();
    void onMedicineDelete();
    void refreshMedicineList();

    void refreshPrescriptionList();
    void onPrescriptionDoubleClicked(QTableWidgetItem *item);
    void onPrescriptionDelete();
    void showPatientPrescriptions();

    void onRegistrationAdd();

private slots:

    void on_btn_add5_clicked();

    void on_btn_search5_clicked();

private:
    Ui::MainWindow *ui;
    DBConnector *dbConnector;
    patient *patientService;
    doctor *doctorService;
    registration *registrationService;
    medicine *medicineService;
    prescription *prescriptionService;
};
#endif // MAINWINDOW_H

