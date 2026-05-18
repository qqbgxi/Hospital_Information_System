#ifndef DBCONNECTOR_H
#define DBCONNECTOR_H

#include <QObject>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>

struct PatientInfo
{
    QString p_id;
    QString p_name;
    QString gender;
    QDate birth;
    QString phone;
    QString address;
};
struct DoctorInfo
{
    QString d_id;
    QString d_name;
    QString gender;
    QString title;
    QString specialty;
    QString dept_id;
};
struct MedicineInfo
{
    QString m_id;
    QString m_name;
    double unit_price;
    int stock;
    QDate productionDate;
    QDate expiryDate;
};
struct RegistrationInfo
{
    QString p_id;
    QString d_id;
    QDate date;
};

class DBConnector : public QObject
{
    Q_OBJECT
public:
    explicit DBConnector(QObject *parent = nullptr);
    ~DBConnector();

    static DBConnector* getInstance();
    bool connectToDatabase(const QString &host, const QString &dbName,
                           const QString &user, const QString &pass);
    QSqlDatabase getDatabase() const;
    QVector<QStringList> getPatientPrescriptionView();
    QVector<QStringList> getPatientPrescriptions(const QString &searchText);
private:
    static DBConnector* instance;
    QSqlDatabase db;
};

#endif // DBCONNECTOR_H
