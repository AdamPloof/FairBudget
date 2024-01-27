#include "person_table.h"
#include <vector>

// PersonTable::Person(QObject *parent, QSqlDatabase &db) : 
//     QSqlTableModel(parent, db)
// {

// }

PersonTable::PersonTable(QObject *parent) {

};

int PersonTable::rowCount(const QModelIndex &parent) const {
    return m_persons.size();
}

int PersonTable::columnCount(const QModelIndex &parent) const {
        if (m_persons.size() == 0) {
        return 0;
    }

    return m_persons.at(0)->fields().size();
}

QVariant PersonTable::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && m_persons.size() > 0) {
       return m_persons.at(index.row())->getData();
    }

    return QVariant();
}

QVariant PersonTable::headerData(
    int section,
    Qt::Orientation orientation,
    int role = Qt::DisplayRole
) const {
    if (m_persons.size() == 0) {
        return QVariant();
    }

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_persons[0]->fields()[section];
    }

    return QVariant();
}

void PersonTable::addRow(std::shared_ptr<ModelInterface> row) {
    m_persons.push_back(row);
}
