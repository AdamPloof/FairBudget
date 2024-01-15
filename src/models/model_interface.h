#ifndef MODEL_INTERFACE_H
#define MODEL_INTERFACE_H

#include <QString>
#include <QList>
#include <vector>

class ModelInterface {
public:
    virtual QString name() = 0;
    virtual std::vector<QString> fields() = 0;
    virtual void addRow(QList<QString>) = 0;
};

#endif // MODEL_INTERFACE_H
