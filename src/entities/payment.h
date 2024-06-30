#ifndef PAYMENT_H
#define PAYMENT_H

#include <QString>
#include <QHash>
#include <QVariant>
#include <memory>

#include "entity_interface.h"
#include "expense.h"
#include "person.h"

class Payment : public EntityInterface {
public:
    static QString name;
    static std::vector<QString> fields;

    Payment();
    QString entityName() const override;
    EntityType entityType() const override;
    std::vector<QString> entityFields() const override;
    void setData(QString field, QVariant val) override;
    void setPaidBy(std::shared_ptr<Person> person);
    void setExpense(std::shared_ptr<Expense> expense);
    const int getId() const override;
    QHash<QString, QVariant> getData(int role = 0) const override;
    QVariant getData(QString field, int role = 0) const override;
    std::shared_ptr<Person> getPaidBy() const;
    std::shared_ptr<Expense> getExpense() const;
private:
    int m_id;
    std::shared_ptr<Person> m_paidBy;
    std::shared_ptr<Expense> m_expense;
    double m_amount;
};

#endif // PAYMENT_H
