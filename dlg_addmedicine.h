#ifndef DLG_ADDMEDICINE_H
#define DLG_ADDMEDICINE_H

#include "dbconnector.h"
#include <QDialog>

namespace Ui {
class Dlg_AddMedicine;
}

class Dlg_AddMedicine : public QDialog
{
    Q_OBJECT

public:
    explicit Dlg_AddMedicine(QWidget *parent = nullptr);
    ~Dlg_AddMedicine();

    void setType(bool isAdd, MedicineInfo info={});

signals:
    void medicineAdded(MedicineInfo info);
    void medicineUpdated(MedicineInfo info);

private slots:
    void on_btn_save_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::Dlg_AddMedicine *ui;
    bool m_isAdd;
    MedicineInfo m_info;
};

#endif // DLG_ADDMEDICINE_H



