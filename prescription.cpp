#include "prescription.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>

prescription::prescription(QSqlDatabase db, QObject *parent)
    : QObject(parent), database(db)
{}

QVector<QStringList> prescription::getAllPrescriptions()
{
    QVector<QStringList> prescriptions;
    QSqlQuery query("SELECT Prescription_id, patient_id, doctor_id, "
                    "issue_date, total_amount FROM Prescription", database);

    while(query.next()) {
        QStringList prescription;
        prescription << query.value(0).toString();  // id
        prescription << query.value(1).toString();  // name
        prescription << query.value(2).toString();  // gender
        prescription << query.value(3).toDate().toString("yyyy-MM-dd");  // birth
        prescription << query.value(4).toString();  // phone
        prescription << query.value(5).toString();  // address
        prescriptions.append(prescription);
    }

    return prescriptions;
}

QVector<QStringList> prescription::getPrescriptionDetails(const QString &prescriptionId)
{
    QVector<QStringList> details;
    QSqlQuery query;
    query.prepare("SELECT m.medicine_name, pd.usage "
                  "FROM PrescriptionDetail pd "
                  "JOIN Medicine m ON pd.medicine_id = m.medicine_id "
                  "WHERE pd.Prescription_id = :prescriptionId");

    query.bindValue(":prescriptionId", prescriptionId);

    if (!query.exec()) {
        qDebug() << "Error fetching prescription details:" << query.lastError().text();
        return details;
    }

    while(query.next()) {
        QStringList detail;
        detail << query.value(0).toString();  // medicine_name
        detail << query.value(1).toString();  // usage
        details.append(detail);
    }

    return details;
}

bool prescription::addPrescription(const QString &id, const QString &p_id, const QString &d_id,
                         const QDate &date, const double &amount)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO Prescription (Prescription_id, patient_id, doctor_id, "
                  "issue_date, total_amount) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(p_id);
    query.addBindValue(d_id);
    query.addBindValue(date);
    query.addBindValue(amount);

    if(!query.exec()) {
        qDebug() << "Add prescription error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool prescription::addDetails(const QString &id, const QString &m_id, const QString &usage)
{
    QSqlQuery query(database);
    query.prepare("INSERT INTO Prescriptiondetail (Prescription_id, medicine_id, `usage`) VALUES (?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(m_id);
    query.addBindValue(usage);

    if(!query.exec()) {
        qDebug() << "Add detail error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool prescription::addPrescriptionWithMedicineCheck(
    const QString &prescriptionId,
    const QString &patientId,
    const QString &doctorId,
    const QDate &issueDate,
    const QStringList &medicineIds,
    const QStringList &usages,
    QString &resultMessage)
{
    QSqlQuery query(database);
    // 准备药品ID和用法字符串
    QString medicineIdsStr = medicineIds.join(",");
    QString usagesStr = usages.join(";");

    query.prepare("CALL AddPrescriptionWithMultipleMedicines(?, ?, ?, ?, ?, ?, @message)");
    query.bindValue(0, prescriptionId);
    query.bindValue(1, patientId);
    query.bindValue(2, doctorId);
    query.bindValue(3, issueDate);
    query.bindValue(4, medicineIdsStr);
    query.bindValue(5, usagesStr);

    if (!query.exec()) {
        resultMessage = "执行存储过程失败: " + query.lastError().text();
        return false;
    }

    if (query.exec("SELECT @message")) {
        if (query.next()) {
            resultMessage = query.value(0).toString();
        }
    } else {
        resultMessage = "获取输出参数失败: " + query.lastError().text();
    }
    qDebug() << resultMessage;
    return !resultMessage.contains("错误") && !resultMessage.contains("不存在") && !resultMessage.contains("不足");
}

bool prescription::deletePrescriptionWithRecords(const QString &prescriptionId)
{
    database.transaction();  // 开始事务
    try {
        // 1. 删除处方明细
        QSqlQuery query(database);
        query.prepare("DELETE FROM PrescriptionDetail WHERE Prescription_id = ?");
        query.addBindValue(prescriptionId);
        if(!query.exec()) throw query.lastError();

        // 2. 删除处方
        query.prepare("DELETE FROM Prescription WHERE Prescription_id = ?");
        query.addBindValue(prescriptionId);
        if(!query.exec()) throw query.lastError();

        database.commit();  // 提交事务
        return true;
    } catch (const QSqlError &e) {
        database.rollback();  // 回滚事务
        qDebug() << "Transaction failed:" << e.text();
        return false;
    }
}

/*
DELIMITER //
CREATE PROCEDURE AddPrescriptionWithMultipleMedicines(
    IN p_prescription_id VARCHAR(20),
    IN p_patient_id VARCHAR(20),
    IN p_doctor_id VARCHAR(20),
    IN p_issue_date DATE,
    IN p_medicine_ids TEXT, -- 用逗号分隔的药品ID
    IN p_usages TEXT,       -- 用分号分隔的用法说明
    OUT p_result VARCHAR(100)
)
BEGIN
    DECLARE i INT DEFAULT 1;
    DECLARE medicine_count INT;
    DECLARE current_medicine_id VARCHAR(20);
    DECLARE current_usage VARCHAR(100);
    DECLARE current_stock INT;
    DECLARE medicine_name VARCHAR(50);
    DECLARE total_amount DECIMAL(10,2) DEFAULT 0;
    DECLARE success BOOLEAN DEFAULT TRUE;
    DECLARE error_message VARCHAR(100) DEFAULT '';

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
GET DIAGNOSTICS CONDITION 1
    @sqlstate = RETURNED_SQLSTATE,
    @errno = MYSQL_ERRNO,
    @text = MESSAGE_TEXT;
SET p_result = CONCAT('错误: ', @sqlstate, ' - ', @text);
SET success = FALSE;
END;

-- 计算药品数量
    SET medicine_count = (LENGTH(p_medicine_ids) - LENGTH(REPLACE(p_medicine_ids, ',', '')) + 1);

-- 开始事务
    START TRANSACTION;

-- 插入处方主表
    INSERT INTO Prescription (Prescription_id, patient_id, doctor_id, issue_date, total_amount)
    VALUES (p_prescription_id, p_patient_id, p_doctor_id, p_issue_date, 0);

-- 循环处理每个药品
        WHILE i <= medicine_count AND success DO
    -- 提取当前药品ID和用法
        SET current_medicine_id = SUBSTRING_INDEX(SUBSTRING_INDEX(p_medicine_ids, ',', i), ',', -1);
SET current_usage = SUBSTRING_INDEX(SUBSTRING_INDEX(p_usages, ';', i), ';', -1);

-- 检查药品库存
      SELECT m.stock, m.medicine_name, m.unit_price
                         INTO current_stock, medicine_name, @unit_price
                       FROM Medicine m
                           WHERE m.medicine_id = current_medicine_id;

IF medicine_name IS NULL THEN
    SET error_message = CONCAT('药品ID ', current_medicine_id, ' 不存在');
SET success = FALSE;
ELSEIF current_stock <= 0 THEN
        SET error_message = CONCAT('药品 "', medicine_name, '" (ID: ', current_medicine_id, ') 库存不足');
SET success = FALSE;
ELSE
    -- 插入处方明细
        INSERT INTO PrescriptionDetail (Prescription_id, medicine_id, `usage`)
    VALUES (p_prescription_id, current_medicine_id, current_usage);

-- 更新药品库存
    UPDATE Medicine SET stock = stock - 1 WHERE medicine_id = current_medicine_id;

-- 累加总金额
    SET total_amount = total_amount + @unit_price;
END IF;

SET i = i + 1;
END WHILE;

-- 更新处方总金额
    UPDATE Prescription
        SET total_amount = total_amount
        WHERE Prescription_id = p_prescription_id;

-- 提交或回滚事务
    IF success THEN
        COMMIT;
SET p_result = CONCAT('处方添加成功，处方号: ', p_prescription_id);
ELSE
    ROLLBACK;
SET p_result = error_message;
END IF;
END //
    DELIMITER ;
*/
