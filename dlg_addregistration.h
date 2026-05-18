#ifndef DLG_ADDREGISTRATION_H
#define DLG_ADDREGISTRATION_H

#include "dbconnector.h"
#include <QDialog>

namespace Ui {
class dlg_addregistration;
}

class dlg_addregistration : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_addregistration(QWidget *parent = nullptr);
    ~dlg_addregistration();

    void setType(bool isAdd, RegistrationInfo info={});
signals:
    void registrationAdded(RegistrationInfo info);
    void registrationUpdated(RegistrationInfo info);

private slots:
    void on_btn_save_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::dlg_addregistration *ui;
    bool m_isAdd;
    RegistrationInfo m_info;
};

#endif // DLG_ADDREGISTRATION_H
