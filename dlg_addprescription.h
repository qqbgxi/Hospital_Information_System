#ifndef DLG_ADDPRESCRIPTION_H
#define DLG_ADDPRESCRIPTION_H

#include"dbconnector.h"
#include "prescription.h"
#include "mainwindow.h"
#include <QDialog>
#include <QDateEdit>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>

namespace Ui {
class dlg_addprescription;
}

class dlg_addprescription : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_addprescription(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~dlg_addprescription();

    QString getPatientId() const;
    QString getDoctorId() const;

private slots:
    void on_addDetailButton_clicked();
    void on_saveButton_clicked();

    void on_btn_del_clicked();

private:
    Ui::dlg_addprescription *ui;
    prescription *prescriptionService ;
    DBConnector *dbConnector;
    MainWindow *mainWindow;
};

#endif // DLG_ADDPRESCRIPTION_H
