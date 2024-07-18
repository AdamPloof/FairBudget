#include "income_period.h"

QString IncomePeriod::name = "income_period";
std::vector<QString> IncomePeriod::fields = {"id", "period", "label"};

IncomePeriod::IncomePeriod() {}

QString IncomePeriod::entityName() const {
    return IncomePeriod::name;
}

EntityType IncomePeriod::entityType() const {
    return EntityType::INCOME_PERIOD;
}

std::vector<QString> IncomePeriod::entityFields() const {
    return IncomePeriod::fields;
}

void IncomePeriod::setData(QString field, QVariant val) {
    if (field == "id") {
        m_id = val.toInt();
    } else if (field == "period") {
        m_period = val.toString();
    } else if (field == "label") {
        m_label = val.toString();
    } else {
        throw std::invalid_argument("Invalid field for IncomePeriod");
    }
}

const int IncomePeriod::getId() const {
    return m_id;
}

QHash<QString, QVariant> IncomePeriod::getData(int role) const {
    return {
        {"id", m_id},
        {"period", m_period},
        {"label", m_label}
    };
}

QVariant IncomePeriod::getData(QString field, int role) const {
    QVariant data;
    if (field == "id") {
        data = m_id;
    } else if (field == "period") {
        data = m_period;
    } else if (field == "label") {
        data = m_label;
    } else {
        throw std::invalid_argument("Invalid field for IncomePeriod");
    }

    return data;
}
