#include "payment_table.h"
#include "../models/payment.h"
#include <vector>

// PaymentTable::PaymentTable(QObject *parent, QSqlDatabase &db) : 
//     QSqlTableModel(parent, db)
// {

// }

PaymentTable::PaymentTable(QObject *parent) {

};

int PaymentTable::rowCount(const QModelIndex &parent) const {
    return m_payments.size();
}

int PaymentTable::columnCount(const QModelIndex &parent) const {
    return Payment::fields.size();
}

QVariant PaymentTable::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole && m_payments.size() > 0) {
       return m_payments.at(index.row())->getData(index.column());
    }

    return QVariant();
}

QVariant PaymentTable::headerData(
    int section,
    Qt::Orientation orientation,
    int role = Qt::DisplayRole
) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return Payment::fields[section];
    }

    return QVariant();
}

void PaymentTable::addRow(std::shared_ptr<ModelInterface> row) {
    m_payments.push_back(row);
}
