#include "expense.h"
#include <vector>

// Expense::Expense(QObject *parent, QSqlDatabase &db) : 
//     QSqlTableModel(parent, db)
// {

// }

Expense::Expense(QObject *parent) : QAbstractTableModel(parent), m_name("expense") {
    m_fields = std::vector<QString> {"id", "description", "amount"};
};

int Expense::rowCount(const QModelIndex &parent) const {
    return m_expenseLines.size();
}

int Expense::columnCount(const QModelIndex &parent) const {
    return m_fields.size();
}

QVariant Expense::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && m_expenseLines.size() > 0) {
       return m_expenseLines[index.row()][index.column()];
    }

    return QVariant();
}

QString Expense::name() {
    return m_name;
}

std::vector<QString> Expense::fields() {
    return m_fields;
}

void Expense::addRow(ExpenseLine row) {
    m_expenseLines.push_back(row);
}
