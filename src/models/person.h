#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QList>
#include <vector>
#include "model_interface.h"

class Person : public ModelInterface {
public:
    static QString name;
    static std::vector<QString> fields;

    Person();
    QString modelName() const override;
    ModelType modelType() const override;
    std::vector<QString> modelFields() const override;
    void setData(QString field, QString val) override;
    const QString getId() const override;
    std::vector<ModelFieldValue> getData() const override;
    QString getData(int idx) const override;

private:
    QString m_id;
    QString m_name;
    QString m_income;
};

#endif // PERSON_H
