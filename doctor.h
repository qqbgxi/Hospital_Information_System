#ifndef DOCTOR_H
#define DOCTOR_H

#include <QObject>
#include <QSqlDatabase>

class doctor : public QObject
{
    Q_OBJECT
public:
    explicit doctor(QSqlDatabase db, QObject *parent = nullptr);

    bool addDoctor(const QString &id, const QString &name, const QString &gender,
                    const QString &title, const QString &specialty, const QString &dept_id);
    bool deleteDoctor(const QString &doctorId);
    bool updatePatient(const QString &id, const QString &name, const QString &gender,
                       const QString &title, const QString &specialty, const QString &dept_id);

    QVector<QStringList> getAllDoctors();
private:
    QSqlDatabase database;
};

#endif // DOCTOR_H


