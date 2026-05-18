#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <QObject>
#include <QSqlDatabase>

class prescription : public QObject
{
    Q_OBJECT
public:
    explicit prescription(QSqlDatabase db, QObject *parent = nullptr);

    QVector<QStringList> getAllPrescriptions();
    QVector<QStringList> getPrescriptionDetails(const QString &prescriptionId);
    bool addPrescription(const QString &id, const QString &p_id, const QString &d_id,
                         const QDate &date, const double &amount);
    bool addDetails(const QString &id, const QString &m_id, const QString &usage);

    bool addPrescriptionWithMedicineCheck(const QString &prescriptionId, const QString &patientId,
                                          const QString &doctorId, const QDate &issueDate,
                                          const QStringList &medicineIds, const QStringList &usages,
                                          QString &resultMessage);
    bool deletePrescriptionWithRecords(const QString &prescriptionId);
signals:

private:
    QSqlDatabase database;

};

#endif // PRESCRIPTION_H




