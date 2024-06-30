#include "person.h"

QString Person::name = "person";
std::vector<QString> Person::fields = {"id", "name", "income", "income_period"};

Person::Person() {}

QString Person::entityName() const {
    return Person::name;
}

EntityType Person::entityType() const {
    return EntityType::PERSON;
}

std::vector<QString> Person::entityFields() const {
    return Person::fields;
}

void Person::setData(QString field, QVariant val) {
    if (field == "id") {
        m_id = val.toInt();
    } else if (field == "name") {
        m_name = val.toString();
    } else if (field == "income") {
        m_income = val.toFloat();
    } else if (field == "income_period") {
        m_incomePeriod = val.toString();
    } else {
        throw std::invalid_argument("Invalid field for Person");
    }
}

const int Person::getId() const {
    return m_id;
}

QHash<QString, QVariant> Person::getData(int role) const {
    return {
        {"id", m_id},
        {"name", m_name},
        {"income", m_income},
        {"income_period", m_incomePeriod}
    };
}

QVariant Person::getData(QString field, int role) const {
    QVariant data;
    if (field == "id") {
        data = m_id;
    } else if (field == "name") {
        data = m_name;
    } else if (field == "income") {
        data = m_income;
    } else if (field == "income_period") {
        data = m_incomePeriod;
    } else {
        throw std::invalid_argument("Invalid field for Person");
    }

    return data;
}
