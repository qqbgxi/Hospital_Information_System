#ifndef DLG_ADDDOCTOR_H
#define DLG_ADDDOCTOR_H

#include "dbconnector.h"
#include <QDialog>

namespace Ui {
class Dlg_AddDoctor;
}

class dlg_adddoctor : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_adddoctor(QWidget *parent = nullptr);
    ~dlg_adddoctor();

    void setType(bool isAdd, DoctorInfo info={});

signals:
    void doctorAdded(DoctorInfo info);
    void doctorUpdated(DoctorInfo info);

private slots:
    void on_btn_save_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::Dlg_AddDoctor *ui;
    bool m_isAdd;
    DoctorInfo m_info;
};

#endif // DLG_ADDDOCTOR_H





