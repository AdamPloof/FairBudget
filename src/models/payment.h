#ifndef PAYMENT_H
#define PAYMENT_H

#include "model_interface.h"

class Payment : public ModelInterface {
public:
    static QString name;
    static std::vector<QString> fields;

    Payment();
    void setData(QString field, QString val) override;
    QList<QString> getData() const override;
    QString getData(int idx) const override;

private:
    // TODO, it would be nice to use the actual data types as they are in the db
    QString m_id;
    QString m_paidBy;
    QString m_expense;
    QString m_amount;
};

#endif // PAYMENT_H
