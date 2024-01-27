#include <QString>
#include <QList>
#include <exception>

#include "expense.h"

QString Expense::name = "expense";
std::vector<QString> Expense::fields = {"id", "description", "amount"};

Expense::Expense() {}

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
