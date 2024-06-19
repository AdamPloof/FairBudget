#ifndef PERSON_H
#define PERSON_H

#include "entity_interface.h"

class Person : public EntityInterface {
public:
    static QString name;
    static std::vector<QString> fields;

    Person();
    QString entityName() const override;
    EntityType entityType() const override;
    std::vector<QString> entityFields() const override;
    void setData(QString field, QVariant val) override;
    const int getId() const override;
    QHash<QString, QVariant> getData() const override;
    QVariant getData(QString field) const override;
private:
    int m_id;
    QString m_name;
    float m_income;
    QString m_incomePeriod;
};

#endif // PERSON_H
