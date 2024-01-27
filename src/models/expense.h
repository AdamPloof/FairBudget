#ifndef EXPENSE_H
#define EXPENSE_H

#include "model_interface.h"

class Expense : public ModelInterface {
public:
    Expense();
    QString name() const override;
    std::vector<QString> fields() const override;
    void setData(QString field, QString val) override;
    QList<QString> getData() const override;

private:
    // TODO, it would be nice to use the actual data types as they are in the db
    QString m_id;
    QString m_description;
    QString m_amount;
    std::vector<QString> m_fields;
};

#endif // EXPENSE_H
