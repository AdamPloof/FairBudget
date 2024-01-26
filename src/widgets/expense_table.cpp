#include "expense_table.h"
#include <vector>

// ExpenseTable::Expense(QObject *parent, QSqlDatabase &db) : 
//     QSqlTableModel(parent, db)
// {

// }

ExpenseTable::ExpenseTable(QObject *parent) {

};

int ExpenseTable::rowCount(const QModelIndex &parent) const {
    return m_expenses.size();
}

int ExpenseTable::columnCount(const QModelIndex &parent) const {
    if (m_expenses.size() == 0) {
        return 0;
    }

    return m_expenses.at(0)->fields().size();
}

QVariant ExpenseTable::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && m_expenses.size() > 0) {
        // TODO: get the right column
       return m_expenses.at(index.row())->getData();
    }

    return QVariant();
}

QVariant ExpenseTable::headerData(
    int section,
    Qt::Orientation orientation,
    int role = Qt::DisplayRole
) const {
    if (m_expenses.size() == 0) {
        return QVariant();
    }

    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return m_expenses[0]->fields()[section];
    }

    return QVariant();
}

void ExpenseTable::addRow(std::shared_ptr<ModelInterface> row) {
    m_expenses.push_back(row);
}
