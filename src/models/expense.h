#ifndef EXPENSE_H
#define EXPENSE_H

#include "model_interface.h"

class Expense : public ModelInterface {
public:
    static QString name;
    static std::vector<QString> fields;

    Expense();
    ModelType modelType() const override;
    void setData(QString field, QString val) override;
    const QString getId() const override;
    QList<QString> getData() const override;
    QString getData(int idx) const override;

private:
    // TODO, it would be nice to use the actual data types as they are in the db
    QString m_id;
    QString m_description;
    QString m_amount;
};

#endif // EXPENSE_H
