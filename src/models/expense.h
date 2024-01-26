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
    int m_id;
    QString m_description;
    int m_amount;
};

#endif // EXPENSE_H
