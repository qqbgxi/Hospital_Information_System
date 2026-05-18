#ifndef DLG_PATIENTPRESCRIPTIONS_H
#define DLG_PATIENTPRESCRIPTIONS_H

#include <QDialog>
#include "dbconnector.h"

namespace Ui {
class dlg_patientprescriptions;
}

class dlg_patientprescriptions : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_patientprescriptions(DBConnector *dbConnector, QWidget *parent = nullptr);
    ~dlg_patientprescriptions();

private:
    Ui::dlg_patientprescriptions *ui;
    DBConnector *dbConnector;

    void loadPatientPrescriptions();
};

#endif // DLG_PATIENTPRESCRIPTIONS_H
