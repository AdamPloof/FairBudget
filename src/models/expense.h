#ifndef EXPENSE_H
#define EXPENSE_H

#include "model_interface.h"

class Expense : public ModelInterface {
public:
    static QString name;
    static std::vector<QString> fields;

    Expense();
    QString modelName() const override;
    ModelType modelType() const override;
    std::vector<QString> modelFields() const override;
    void setData(QString field, QString val) override;
    const QString getId() const override;
    std::vector<ModelFieldValue> getData() const override;
    QString getData(int idx) const override;

private:
    // TODO, it would be nice to use the actual data types as they are in the db
    QString m_id;
    QString m_description;
    QString m_amount;
};

#endif // EXPENSE_H
