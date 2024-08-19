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
        int intVal = val.toInt();
        if (intVal > 0) {
            m_id = intVal;
        }
    } else if (field == "description") {
        QString strVal = val.toString();
        if (strVal != "") {
            m_description = strVal;
        }
    } else if (field == "amount") {
        double dblVal = val.toDouble();
        if (dblVal > 0.0) {
            m_amount = dblVal;
        }
    } else {
        throw std::invalid_argument("Invalid field for Expense");
    }
}

const int Expense::getId() const {
    return m_id;
}

QHash<QString, QVariant> Expense::getData(int role) const {
    return {
        {"id", m_id},
        {"description", m_description},
        {"amount", m_amount}
    };
}

QVariant Expense::getData(QString field, int role) const {
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
