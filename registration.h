#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QObject>
#include <QSqlDatabase>

class registration : public QObject
{
    Q_OBJECT
public:
    explicit registration(QSqlDatabase db, QObject *parent = nullptr);

    QVector<QStringList> getAllRegistrations();
    bool addRegistration(const QString &p_id, const QString &d_id, const QDate &time);
private:
    QSqlDatabase database;
};

#endif // REGISTRATION_H

