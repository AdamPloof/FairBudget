#include "expense.h"

QString Expense::name = "expense";
std::vector<QString> Expense::fields = {"id", "description", "amount"};

Expense::Expense() {}

QString Expense::entityName() const {
    return Expense::name;
}

EntityType Expense::entityType() const {
    return EntityType::EXPENSE;
}

std::vector<QString> Expense::entityFields() const {
    return Expense::fields;
}

void Expense::setData(QString field, QVariant val) {
    if (field == "id") {
        m_id = val.toInt();
    } else if (field == "description") {
        m_description = val.toString();
    } else if (field == "amount") {
        m_amount = val.toFloat();
    } else {
        throw std::invalid_argument("Invalid field for Expense");
    }
}

const int Expense::getId() const {
    return m_id;
}

QHash<QString, QVariant> Expense::getData() const {
    return {
        {"id", m_id},
        {"description", m_description},
        {"amount", m_amount}
    };
}

QVariant Expense::getData(QString field) const {
    QVariant data;
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
