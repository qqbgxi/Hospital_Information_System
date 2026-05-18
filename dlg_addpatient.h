#ifndef DLG_ADDPATIENT_H
#define DLG_ADDPATIENT_H

#include "dbconnector.h"
#include <QDialog>

namespace Ui {
class Dlg_AddPatient;
}

class dlg_addpatient : public QDialog
{
    Q_OBJECT
public:
    explicit dlg_addpatient(QWidget *parent = nullptr);
    ~dlg_addpatient();

    void setType(bool isAdd, PatientInfo info={});

signals:
    void patientAdded(PatientInfo info);
    void patientUpdated(PatientInfo info);

private slots:
    void on_btn_save_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::Dlg_AddPatient *ui;
    bool m_isAdd;
    PatientInfo m_info;
};

#endif // DLG_ADDPATIENT_H



