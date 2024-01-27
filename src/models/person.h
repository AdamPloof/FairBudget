#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include <QList>
#include <vector>
#include "model_interface.h"

class Person : public ModelInterface {
public:
    Person();
    QString name() const override;
    std::vector<QString> fields() const override;
    void setData(QString field, QString val) override;
    QList<QString> getData() const override;

private:
    QString m_id;
    QString m_name;
    QString m_income;
    std::vector<QString> m_fields;
};

#endif // PERSON_H
