#include "doctor.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

doctor::doctor(QSqlDatabase db, QObject *parent)
    : QObject(parent), database(db)
{}

bool doctor::addDoctor(const QString &id, const QString &name, const QString &gender,
                         const QString &title, const QString &specialty, const QString &dept_id)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO Doctor (doctor_id, doctor_name, gender, "
                  "title, specialty, dept_id) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(name);
    query.addBindValue(gender);
    query.addBindValue(title);
    query.addBindValue(specialty);
    query.addBindValue(dept_id);

    if(!query.exec()) {
        qDebug() << "Add doctor error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool doctor::updatePatient(const QString &id, const QString &name, const QString &gender, const QString &title, const QString &specialty, const QString &dept_id)
{

}

bool doctor::deleteDoctor(const QString &doctorId)
{
    database.transaction();  // 开始事务

    try {
        // 1. 删除处方明细
        QSqlQuery query(database);
        query.prepare("DELETE FROM PrescriptionDetail WHERE prescription_id IN "
                      "(SELECT prescription_id FROM Prescription WHERE patient_id = ?)");
        query.addBindValue(doctorId);
        if(!query.exec()) throw query.lastError();

        // 2. 删除处方
        query.prepare("DELETE FROM Prescription WHERE patient_id = ?");
        query.addBindValue(doctorId);
        if(!query.exec()) throw query.lastError();

        // 3. 删除挂号记录
        query.prepare("DELETE FROM Registration WHERE patient_id = ?");
        query.addBindValue(doctorId);
        if(!query.exec()) throw query.lastError();

        // 4. 删除住院记录
        query.prepare("DELETE FROM Hospitalization WHERE patient_id = ?");
        query.addBindValue(doctorId);
        if(!query.exec()) throw query.lastError();

        // 5. 删除患者
        query.prepare("DELETE FROM Doctor WHERE doctor_id = ?");
        query.addBindValue(doctorId);
        if(!query.exec()) throw query.lastError();

        database.commit();  // 提交事务
        return true;
    } catch (const QSqlError &e) {
        database.rollback();  // 回滚事务
        qDebug() << "Transaction failed:" << e.text();
        return false;
    }
}

QVector<QStringList> doctor::getAllDoctors()
{
    QVector<QStringList> doctors;
    QSqlQuery query("SELECT doctor_id, doctor_name, gender, "
                    "title, specialty, dept_id FROM Doctor", database);

    while(query.next()) {
        QStringList doctor;
        doctor << query.value(0).toString();  // id
        doctor << query.value(1).toString();  // name
        doctor << query.value(2).toString();  // gender
        doctor << query.value(3).toString();  // title
        doctor << query.value(4).toString();  // specialty
        doctor << query.value(5).toString();  // dept_id
        doctors.append(doctor);
    }

    return doctors;
}

