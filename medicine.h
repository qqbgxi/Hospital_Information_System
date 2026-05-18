#ifndef MEDICINE_H
#define MEDICINE_H

#include <QObject>
#include <QSqlDatabase>

class medicine : public QObject
{
    Q_OBJECT
public:
    explicit medicine(QSqlDatabase db, QObject *parent = nullptr);

    bool addMedicine(const QString &id, const QString &name, const double &unit_price,
                    const int &stock, const QDate &productionDate, const QDate &expiryDate);

    QVector<QStringList> getAllMedicines();

private:
    QSqlDatabase database;
signals:
};

#endif // MEDICINE_H




