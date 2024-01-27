#include <QString>
#include <QList>
#include <exception>

#include "person.h"

Person::Person() : m_fields {"id", "name", "income"} {

}

QString Person::name() const {
    return "person";
}

std::vector<QString> Person::fields() const {
    return m_fields;
}

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
