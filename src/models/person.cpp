#include <QString>
#include <QList>
#include <exception>

#include "person.h"

QString Person::name = "person";
std::vector<QString> Person::fields = {"id", "name", "income"};

Person::Person() {}

void Person::setData(QString field, QString val) {
    if (field == "id") {
        m_id = val;
    } else if (field == "name") {
        m_name = val;
    } else if (field == "income") {
        m_income = val;
    } else {
        throw std::invalid_argument("Invalid field for Person");
    }
}

QList<QString> Person::getData() const {
    QList<QString> data = {m_id, m_name, m_income};

    return data;
}

QString Person::getData(int idx) const {
    QString field = Person::fields[idx];
    QString data;
    if (field == "id") {
        data = m_id;
    } else if (field == "name") {
        data = m_name;
    } else if (field == "income") {
        data = m_income;
    } else {
        throw std::invalid_argument("Invalid field for Expense");
    }

    return data;
}
