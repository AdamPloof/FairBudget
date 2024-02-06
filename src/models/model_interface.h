#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include <QString>
#include <QList>
#include <vector>

enum class ModelType {
    EXPENSE,
    PERSON,
    PAYMENT
};

class ModelInterface {
public:
    virtual ModelType modelType() const = 0;
    virtual void setData(QString field, QString val) = 0;
    virtual const QString getId() const = 0;
    virtual QList<QString> getData() const = 0;
    virtual QString getData(int idx) const = 0;
};

#endif // MODEL_INTERFACE_H
