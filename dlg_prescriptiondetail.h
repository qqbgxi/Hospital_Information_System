#ifndef DLG_PRESCRIPTIONDETAIL_H
#define DLG_PRESCRIPTIONDETAIL_H

#include <QDialog>
#include <QVector>
#include <QStringList>
#include <QTableWidget>

namespace Ui {
class dlg_prescriptiondetail;
}

class dlg_prescriptiondetail : public QDialog
{
    Q_OBJECT

public:
    explicit dlg_prescriptiondetail(QWidget *parent = nullptr);
    ~dlg_prescriptiondetail();

    void setPrescriptionDetails(const QVector<QStringList> &details);
private:
    Ui::dlg_prescriptiondetail *ui;
};

#endif // DLG_PRESCRIPTIONDETAIL_H
