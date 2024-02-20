#include <QString>
#include <QList>
#include <exception>

#include "payment.h"

QString Payment::name = "payment";
std::vector<QString> Payment::fields = {"id", "expense", "paid_by", "amount"};

Payment::Payment() {}

QString Payment::modelName() const {
    return Payment::name;
}

ModelType Payment::modelType() const {
    return ModelType::PAYMENT;
}

std::vector<QString> Payment::modelFields() const {
    return Payment::fields;
}

void Payment::setData(QString field, QString val) {
    if (field == "id") {
        m_id = val;
    } else if (field == "expense") {
        m_expense = val;
    } else if (field == "paid_by") {
        m_paidBy = val;
    } else if (field == "amount") {
        m_amount = val;
    } else {
        throw std::invalid_argument("Invalid field for Expense");
    }
}

const QString Payment::getId() const {
    return m_id;
}

std::vector<ModelFieldValue> Payment::getData() const {
    std::vector<ModelFieldValue> data;
    data.push_back({"id", m_id});
    data.push_back({"expense", m_expense});
    data.push_back({"paid_by", m_paidBy});
    data.push_back({"amount", m_amount});

    return data;
}

QString Payment::getData(int idx) const {
    QString field = Payment::fields[idx];
    QString data;
    if (field == "id") {
        data = m_id;
    } else if (field == "expense") {
        data = m_expense;
    } else if (field == "paid_by") {
        data = m_paidBy;
    } else if (field == "amount") {
        data = m_amount;
    } else {
        throw std::invalid_argument("Invalid field for Expense");
    }

    return data;
}
