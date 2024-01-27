#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include <QString>
#include <QList>
#include <vector>

class ModelInterface {
public:
    virtual void setData(QString field, QString val) = 0;
    virtual QList<QString> getData() const = 0;
    virtual QString getData(int idx) const = 0;
};

#endif // MODEL_INTERFACE_H
