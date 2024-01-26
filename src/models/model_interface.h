#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include <QString>
#include <QList>
#include <vector>

class ModelInterface {
public:
    virtual QString name() const = 0;
    virtual std::vector<QString> fields() const = 0;
    virtual void setData(QString field, QString val) = 0;
    virtual QList<QString> getData() const = 0;
};

#endif // MODEL_INTERFACE_H
