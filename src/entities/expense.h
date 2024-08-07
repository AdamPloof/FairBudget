#ifndef EXPENSE_H
#define EXPENSE_H

#include <QString>
#include <QHash>
#include <QVariant>

#include "entity_interface.h"

class Expense : public EntityInterface {
public:
    static QString name;
    static std::vector<QString> fields;

    Expense();
    QString entityName() const override;
    EntityType entityType() const override;
    std::vector<QString> entityFields() const override;
    void setData(QString field, QVariant val) override;
    const int getId() const override;
    QHash<QString, QVariant> getData(int role = 0) const override;
    QVariant getData(QString field, int role = 0) const override;
private:
    int m_id;
    QString m_description;
    double m_amount;
};

#endif // EXPENSE_H
