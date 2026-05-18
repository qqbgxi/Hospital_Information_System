#include "dbconnector.h"
#include <QSqlDatabase>
#include <QMessageBox>

DBConnector* DBConnector::instance = nullptr;

DBConnector::DBConnector(QObject *parent) : QObject(parent)
{
}

DBConnector::~DBConnector()
{
    if(db.isOpen()) {
        db.close();
    }
}

bool DBConnector::connectToDatabase(const QString &host, const QString &dbName,
                                    const QString &user, const QString &pass)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(host);
    db.setDatabaseName(dbName);
    db.setUserName(user);
    db.setPassword(pass);

    if(!db.open()) {
        qDebug() << "Database connection error:" << db.lastError().text();
        return false;
    }
    return true;
}

QSqlDatabase DBConnector::getDatabase() const
{
    return db;
}

DBConnector* DBConnector::getInstance()
{
    if (!instance) {
        instance = new DBConnector();
    }
    return instance;
}

QVector<QStringList> DBConnector::getPatientPrescriptions(const QString &searchText)
{
    QVector<QStringList> result;
    QSqlQuery query(db);

    QString queryText = "SELECT * FROM PatientPrescriptions WHERE "
                        "patient_id LIKE :searchText OR patient_name LIKE :searchText";
    query.prepare(queryText);
    query.bindValue(":searchText", "%" + searchText + "%");

    if (!query.exec()) {
        qDebug() << "Failed to execute view query:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        QStringList row;
        row << query.value("Prescription_id").toString()
            << query.value("patient_id").toString()
            << query.value("doctor_id").toString()
            << query.value("issue_date").toDate().toString("yyyy-MM-dd")
            << query.value("total_amount").toString();
        result.append(row);
    }

    return result;
}

QVector<QStringList> DBConnector::getPatientPrescriptionView()
{
    QVector<QStringList> result;
    QSqlQuery query(db);

    if (!query.exec("SELECT * FROM PatientPrescriptionView")) {
        qDebug() << "Failed to execute view query:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        QStringList row;
        row << query.value("patient_id").toString()
            << query.value("patient_name").toString()
            << query.value("gender").toString()
            << query.value("prescription_count").toString()
            << QString::number(query.value("total_spent").toDouble(), 'f', 2);
        result.append(row);
    }

    return result;
}

/*
CREATE VIEW PatientPrescriptions AS
    SELECT
        pr.Prescription_id,
        p.patient_id,
        p.patient_name,
        pr.doctor_id,
        pr.issue_date,
        pr.total_amount
    FROM
        Patient p LEFT JOIN Prescription pr ON p.patient_id = pr.patient_id;
*/
