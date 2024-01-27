#ifndef PERSON_TABLE_H
#define PERSON_TABLE_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include "../models/person.h"
#include "../models/model_interface.h"
#include "table_widget_interface.h"

class PersonTable : public QAbstractTableModel, public TableWidgetInterface {
    Q_OBJECT

    public:
        explicit PersonTable(QObject *parent = nullptr);
        // ~PersonTable();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        void addRow(std::shared_ptr<ModelInterface> row) override;

    private:
        QList<std::shared_ptr<ModelInterface>> m_persons;
};

#endif // PERSON_TABLE_H
