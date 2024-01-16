#include "person.h"
#include <vector>

// Person::Person(QObject *parent, QSqlDatabase &db) : 
//     QSqlTableModel(parent, db)
// {

// }

Person::Person(QObject *parent) : QAbstractTableModel(parent), m_name("person") {
    m_fields = std::vector<QString> {"id", "name", "income"};
};

int Person::rowCount(const QModelIndex &parent) const {
    return m_rows.size();
}

int Person::columnCount(const QModelIndex &parent) const {
    return m_fields.size();
}

QVariant Person::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && m_rows.size() > 0) {
       return m_rows[index.row()][index.column()];
    }

    return QVariant();
}

QVariant Person::headerData(
    int section,
    Qt::Orientation orientation,
    int role = Qt::DisplayRole
) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_fields[section];
    }

    return QVariant();
}

QString Person::name() {
    return m_name;
}

std::vector<QString> Person::fields() {
    return m_fields;
}

void Person::addRow(PersonRow row) {
    m_rows.push_back(row);
}
