#include "patient.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>

patient::patient(QSqlDatabase db, QObject *parent)
    : QObject(parent), database(db)
{
}

bool patient::addPatient(const QString &id, const QString &name, const QString &gender,
                                const QDate &birthDate, const QString &phone, const QString &address)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO Patient (patient_id, patient_name, gender, "
                  "birth_date, phone, address) VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(name);
    query.addBindValue(gender);
    query.addBindValue(birthDate);
    query.addBindValue(phone);
    query.addBindValue(address);

    if(!query.exec()) {
        qDebug() << "Add patient error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool patient::updatePatient(const QString &id, const QString &name, const QString &gender,
                            const QDate &birthDate, const QString &phone, const QString &address)
{
    QSqlQuery query(database);
    query.prepare("UPDATE Patient SET patient_name = ?, gender = ?, "
                  "birth_date = ?, phone = ?, address = ? WHERE patient_id = ?");
    query.addBindValue(name);
    query.addBindValue(gender);
    query.addBindValue(birthDate);
    query.addBindValue(phone);
    query.addBindValue(address);
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "Update patient error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool patient::deletePatientWithRecords(const QString &patientId)
{
    database.transaction();  // 开始事务

    try {
        // 1. 删除处方明细
        QSqlQuery query(database);
        query.prepare("DELETE FROM PrescriptionDetail WHERE prescription_id IN "
                      "(SELECT prescription_id FROM Prescription WHERE patient_id = ?)");
        query.addBindValue(patientId);
        if(!query.exec()) throw query.lastError();

        // 2. 删除处方
        query.prepare("DELETE FROM Prescription WHERE patient_id = ?");
        query.addBindValue(patientId);
        if(!query.exec()) throw query.lastError();

        // 3. 删除挂号记录
        query.prepare("DELETE FROM Registration WHERE patient_id = ?");
        query.addBindValue(patientId);
        if(!query.exec()) throw query.lastError();

        // 4. 删除住院记录
        query.prepare("DELETE FROM Hospitalization WHERE patient_id = ?");
        query.addBindValue(patientId);
        if(!query.exec()) throw query.lastError();

        // 5. 删除患者
        query.prepare("DELETE FROM Patient WHERE patient_id = ?");
        query.addBindValue(patientId);
        if(!query.exec()) throw query.lastError();

        database.commit();  // 提交事务
        return true;
    } catch (const QSqlError &e) {
        database.rollback();  // 回滚事务
        qDebug() << "Transaction failed:" << e.text();
        return false;
    }
}

QVector<QStringList> patient::getAllPatients()
{
    QVector<QStringList> patients;
    QSqlQuery query("SELECT patient_id, patient_name, gender, "
                    "birth_date, phone, address FROM Patient", database);

    while(query.next()) {
        QStringList patient;
        patient << query.value(0).toString();  // id
        patient << query.value(1).toString();  // name
        patient << query.value(2).toString();  // gender
        patient << query.value(3).toDate().toString("yyyy-MM-dd");  // birth
        patient << query.value(4).toString();  // phone
        patient << query.value(5).toString();  // address
        patients.append(patient);
    }

    return patients;
}

