#include <QString>
#include <QList>
#include <exception>

#include "person.h"

QString Person::name = "person";
std::vector<QString> Person::fields = {"id", "name", "income"};

Person::Person() {}

QString Person::modelName() const {
    return Person::name;
}

ModelType Person::modelType() const {
    return ModelType::PERSON;
}

std::vector<QString> Person::modelFields() const {
    return Person::fields;
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

const QString Person::getId() const {
    return m_id;
}

std::vector<ModelFieldValue> Person::getData() const {
    std::vector<ModelFieldValue> data;
    data.push_back({"id", m_id});
    data.push_back({"name", m_name});
    data.push_back({"income", m_income});

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
