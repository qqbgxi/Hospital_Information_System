#include "registration.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>

registration::registration(QSqlDatabase db, QObject *parent)
    : QObject(parent), database(db)
{}

bool registration::addRegistration(const QString &p_id, const QString &d_id, const QDate &time)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO Registration (patient_id, doctor_id, time) VALUES (?, ?, ?)");
    query.addBindValue(p_id);
    query.addBindValue(d_id);
    query.addBindValue(time);

    if(!query.exec()) {
        qDebug() << "Add registration error:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<QStringList> registration::getAllRegistrations()
{
    QVector<QStringList> registrations;
    QSqlQuery query("SELECT patient_id, doctor_id, time FROM Registration", database);

    while(query.next()) {
        QStringList registration;
        registration << query.value(0).toString();  // p_id
        registration << query.value(1).toString();  // d_id
        registration << query.value(2).toDate().toString("yyyy-MM-dd");  // time
        registrations.append(registration);
    }

    return registrations;
}
