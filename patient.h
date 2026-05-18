#ifndef PATIENT_H
#define PATIENT_H

#include <QObject>
#include <QSqlDatabase>

class patient : public QObject
{
    Q_OBJECT
public:
    explicit patient(QSqlDatabase db, QObject *parent = nullptr);

    bool addPatient(const QString &id, const QString &name, const QString &gender,
                    const QDate &birthDate, const QString &phone, const QString &address);
    bool updatePatient(const QString &id, const QString &name, const QString &gender,
                    const QDate &birthDate, const QString &phone, const QString &address);
    bool deletePatientWithRecords(const QString &patientId);

    QVector<QStringList> getAllPatients();

private:
    QSqlDatabase database;
};


#endif // PATIENT_H

