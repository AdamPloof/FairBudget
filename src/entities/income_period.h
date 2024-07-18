#ifndef INCOME_PERIOD_H
#define INCOME_PERIOD_H

#include <QString>
#include <QHash>
#include <QVariant>

#include "entity_interface.h"

class IncomePeriod : public EntityInterface {
public:
    static QString name;
    static std::vector<QString> fields;

    IncomePeriod();
    QString entityName() const override;
    EntityType entityType() const override;
    std::vector<QString> entityFields() const override;
    void setData(QString field, QVariant val) override;
    const int getId() const override;
    QHash<QString, QVariant> getData(int role = 0) const override;
    QVariant getData(QString field, int role = 0) const override;
private:
    int m_id;
    QString m_period;
    QString m_label;
};

#endif // INCOME_PERIOD_H
