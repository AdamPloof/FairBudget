#include "expense_table.h"
#include "../models/expense.h"
#include "../services/persistence_manager.h"
#include <vector>

// ExpenseTable::Expense(QObject *parent, QSqlDatabase &db) : 
//     QSqlTableModel(parent, db)
// {

// }

ExpenseTable::ExpenseTable(PersistenceManager* persistenceManager, QObject *parent) :
    m_persistenceManager(persistenceManager),
    showIndex(false) {};

int ExpenseTable::rowCount(const QModelIndex &parent) const {
    return m_expenses.size();
}

int ExpenseTable::columnCount(const QModelIndex &parent) const {
    return Expense::fields.size();
}

// TODO: we don't want to be able to edit the index column.
// Should probably make showIndex into an instance variable to
// decide how to format and set up that column.
QVariant ExpenseTable::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && m_expenses.size() > 0) {
       return m_expenses.at(index.row())->getData(index.column());
    }

    return QVariant();
}

QVariant ExpenseTable::headerData(
    int section,
    Qt::Orientation orientation,
    int role = Qt::DisplayRole
) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (!showIndex && section == 0) {
            return QVariant();
        }

        return Expense::fields[section];
    }

    return QVariant();
}

Qt::ItemFlags ExpenseTable::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ExpenseTable::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        m_expenses.at(index.row())->setData(Expense::fields.at(index.column()), value.toString());

        // TODO: there's a out of bounds index problem in persistence manager with this call
        // m_persistenceManager->update(m_expenses.at(index.row()));
        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

void ExpenseTable::addRow(std::shared_ptr<ModelInterface> row) {
    m_expenses.push_back(row);
}
