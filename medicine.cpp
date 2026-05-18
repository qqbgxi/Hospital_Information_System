#include "medicine.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QDate>

medicine::medicine(QSqlDatabase db, QObject *parent)
    : QObject(parent), database(db)
{}

bool medicine::addMedicine(const QString &id, const QString &name,
                                  const double &unit_price, const int &stock,
                                  const QDate &productionDate, const QDate &expiryDate)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Medicine (medicine_id, medicine_name, unit_price, "
                  "stock, production_date, expiry_date) "
                  "VALUES (:id, :name, :price, :stock, :prodDate, :expDate)");

    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":price", unit_price);
    query.bindValue(":stock", stock);
    query.bindValue(":prodDate", productionDate);
    query.bindValue(":expDate", expiryDate);

    if (!query.exec()) {
        // 处理触发器抛出的错误
        QString errorText = query.lastError().text();

        qDebug() << "errorText:" << errorText;

        if (errorText.contains("unit")) {
            QMessageBox::warning(nullptr, "添加失败", "药品单价必须大于零");
        } else if (errorText.contains("stock")) {
            QMessageBox::warning(nullptr, "添加失败", "药品库存不能为负数");
        } else if (errorText.contains("Production")) {
            QMessageBox::warning(nullptr, "添加失败", "药品生产日期必须早于有效日期");
        } else {
            QMessageBox::critical(nullptr, "数据库错误",
                                  "添加药品时发生错误: " + query.lastError().text());
        }
        return false;
    }

    QMessageBox::information(nullptr, "成功", "药品添加成功!");
    return true;
}

QVector<QStringList> medicine::getAllMedicines()
{
    QVector<QStringList> medicines;
    QSqlQuery query("SELECT medicine_id, medicine_name, unit_price, "
                    "stock, production_date, expiry_date FROM Medicine", database);

    while(query.next()) {
        QStringList medicine;
        medicine << query.value(0).toString();  // id
        medicine << query.value(1).toString();  // name
        medicine << QString::number(query.value(2).toDouble(), 'f', 2);  // unit_price
        medicine << QString::number(query.value(3).toInt());  // stock
        medicine << query.value(4).toDate().toString("yyyy-MM-dd");  // production_date
        medicine << query.value(5).toDate().toString("yyyy-MM-dd");  // expiry_date
        medicines.append(medicine);
    }

    return medicines;
}

/*
DELIMITER //

            CREATE TRIGGER before_medicine_insert
                BEFORE INSERT ON Medicine
                    FOR EACH ROW
                        BEGIN
    -- 检查药品单价是否大于零
        IF NEW.unit_price <= 0 THEN
        SIGNAL SQLSTATE '45000'
    SET MESSAGE_TEXT = '药品单价必须大于零';
END IF;

-- 检查库存是否大于等于零
        IF NEW.stock < 0 THEN
        SIGNAL SQLSTATE '45001'
    SET MESSAGE_TEXT = '药品库存不能为负数';
END IF;

-- 检查生产日期是否小于有效日期
        IF NEW.production_date IS NOT NULL AND NEW.expiry_date IS NOT NULL THEN
            IF NEW.production_date >= NEW.expiry_date THEN
           SIGNAL SQLSTATE '45002'
       SET MESSAGE_TEXT = '药品生产日期必须早于有效日期';
END IF;
END IF;
END //
*/
