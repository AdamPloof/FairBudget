#include <QString>
#include <QList>
#include <exception>

#include "expense.h"

QString Expense::name = "expense";
std::vector<QString> Expense::fields = {"id", "description", "amount"};

Expense::Expense() {}

QString Expense::modelName() const {
    return Expense::name;
}

ModelType Expense::modelType() const {
    return ModelType::EXPENSE;
}

std::vector<QString> Expense::modelFields() const {
    return Expense::fields;
}

void Expense::setData(QString field, QString val) {
    if (field == "id") {
        m_id = val;
    } else if (field == "description") {
        m_description = val;
    } else if (field == "amount") {
        m_amount = val;
    } else {
        throw std::invalid_argument("Invalid field for Expense");
    }
}

const QString Expense::getId() const {
    return m_id;
}

std::vector<ModelFieldValue> Expense::getData() const {
    std::vector<ModelFieldValue> data;
    data.push_back({"id", m_id});
    data.push_back({"description", m_description});
    data.push_back({"amount", m_amount});

    return data;
}

QString Expense::getData(int idx) const {
    QString field = Expense::fields[idx];
    QString data;
    if (field == "id") {
        data = m_id;
    } else if (field == "description") {
        data = m_description;
    } else if (field == "amount") {
        data = m_amount;
    } else {
        throw std::invalid_argument("Invalid field for Expense");
    }

    return data;
}
