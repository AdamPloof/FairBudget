#include <QString>
#include <QList>
#include <exception>

#include "expense.h"

Expense::Expense() : m_fields {"id", "description", "amount"} {

}

QString Expense::name() const {
    return "expense";
}

std::vector<QString> Expense::fields() const {
    return m_fields;
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

QList<QString> Expense::getData() const {
    QList<QString> data = {m_id, m_description, m_amount};

    return data;
}
