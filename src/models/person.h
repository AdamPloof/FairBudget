#ifndef PERSON_H
#define PERSON_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include "model_interface.h"

class Person : public QAbstractTableModel, public ModelInterface {
    Q_OBJECT

    using PersonRow = QList<QString>;

    public:
        explicit Person(QObject *parent = nullptr);
        // ~Person();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QString name() override;
        std::vector<QString> fields() override;
        void addRow(PersonRow row) override;

    private:
        QList<PersonRow> m_rows;
        QString m_name;
        std::vector<QString> m_fields;
};

#endif // PERSON_H
