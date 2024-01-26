#include "person_table.h"
#include <vector>

// PersonTable::Person(QObject *parent, QSqlDatabase &db) : 
//     QSqlTableModel(parent, db)
// {

// }

PersonTable::PersonTable(QObject *parent) : QAbstractTableModel(parent), m_name("person") {
    m_fields = std::vector<QString> {"id", "name", "income"};
};

int PersonTable::rowCount(const QModelIndex &parent) const {
    return m_rows.size();
}

int PersonTable::columnCount(const QModelIndex &parent) const {
    return m_fields.size();
}

QVariant PersonTable::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && m_rows.size() > 0) {
       return m_rows[index.row()][index.column()];
    }

    return QVariant();
}

QVariant PersonTable::headerData(
    int section,
    Qt::Orientation orientation,
    int role = Qt::DisplayRole
) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_fields[section];
    }

    return QVariant();
}

QString PersonTable::name() {
    return m_name;
}

std::vector<QString> PersonTable::fields() {
    return m_fields;
}

void PersonTable::addRow(PersonRow row) {
    m_rows.push_back(row);
}
