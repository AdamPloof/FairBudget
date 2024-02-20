#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include <QString>
#include <QVariant>
#include <QList>
#include <vector>

enum class ModelType {
    EXPENSE,
    PERSON,
    PAYMENT
};

struct ModelFieldValue {
    const QString field;
    const QVariant value;
};

class ModelInterface {
public:
    virtual QString modelName() const = 0;
    virtual ModelType modelType() const = 0;
    virtual std::vector<QString> modelFields() const = 0;
    virtual void setData(QString field, QString val) = 0;
    virtual const QString getId() const = 0;
    virtual std::vector<ModelFieldValue> getData() const = 0;
    virtual QString getData(int idx) const = 0;
};

#endif // MODEL_INTERFACE_H
