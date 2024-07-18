#ifndef ENTITY_INTERFACE_H
#define ENTITY_INTERFACE_H

#include <QString>
#include <QVariant>
#include <QHash>
#include <vector>

enum class EntityType {
    EXPENSE,
    PERSON,
    PAYMENT,
    INCOME_PERIOD
};

class EntityInterface {
    public:
        virtual QString entityName() const = 0;
        virtual EntityType entityType() const = 0;
        virtual std::vector<QString> entityFields() const = 0;
        virtual void setData(QString field, QVariant val) = 0;
        virtual const int getId() const = 0;
        virtual QHash<QString, QVariant> getData(int role = 0) const = 0;
        virtual QVariant getData(QString field, int role = 0) const = 0;
};

#endif // ENTITY_INTERFACE_H
