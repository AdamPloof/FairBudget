#include <string>
#include <sstream>
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
        m_income = val.toDouble();
    } else {
        std::stringstream err;
        err << "Invalid field for Person: ";
        err << field.toStdString();
        throw std::invalid_argument(err.str());
    }
}

void Person::setIncomePeriod(std::shared_ptr<IncomePeriod> period) {
    m_incomePeriod = period;
}

const int Person::getId() const {
    return m_id;
}

QHash<QString, QVariant> Person::getData(int role) const {
    if (role == Qt::DisplayRole) {
        return {
            {"id", m_id},
            {"name", m_name},
            {"income", m_income},
            {"income_period", m_incomePeriod->getData("label")}
        };
    } else if (role == Qt::UserRole || role == Qt::EditRole) {
        return {
            {"id", m_id},
            {"name", m_name},
            {"income", m_income},
            {"income_period", m_incomePeriod->getData("id")}
        };
    } else {
        std::stringstream err;
        err << "Invalid role provided for getData: " << role;
        throw std::invalid_argument(err.str());
    }
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
        if (role == Qt::DisplayRole) {
            data = m_incomePeriod->getData("label");
        } else if (role == Qt::UserRole || role == Qt::EditRole) {
            data = m_incomePeriod->getData("id");
        } else {
            std::stringstream err;
            err << "Invalid role provided for getData: " << role;
            throw std::invalid_argument(err.str());
        }
    } else {
        std::stringstream err;
        err << "Invalid field for Person: ";
        err << field.toStdString();
        throw std::invalid_argument(err.str());
    }

    return data;
}

std::shared_ptr<IncomePeriod> Person::getIncomePeriod() const {
    return m_incomePeriod;
}
