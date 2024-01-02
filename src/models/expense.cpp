#include "expense.h"

// Expense::Expense(QObject *parent, QSqlDatabase &db) : 
//     QSqlTableModel(parent, db)
// {

// }

Expense::Expense(QObject *parent) : QAbstractTableModel(parent) {};

int Expense::rowCount(const QModelIndex &parent) const {
    return 3;
}

int Expense::columnCount(const QModelIndex &parent) const {
    return 4;
}

QVariant Expense::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
       return QString("Row%1, Column%2")
                   .arg(index.row() + 1)
                   .arg(index.column() +1);

    return QVariant();
}
