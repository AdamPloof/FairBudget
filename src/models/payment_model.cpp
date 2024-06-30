#include <QList>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "payment_model.h"
#include "../services/entity_manager.h"
#include "../entities/payment.h"
#include "../entities/entity_interface.h"

PaymentModel::PaymentModel(
    std::shared_ptr<EntityManager> em,
    QObject *parent
) : m_entityManager(em) {

}

// PaymentModel::~ExpenseModel() {
    
// }

void PaymentModel::load() {
    QString qStr =
        "SELECT pmt.id, "
        "    pmt.paid_by AS person_id, "
        "    prsn.name AS person_name, "
        "    prsn.income AS person_income, "
        "    prsn.income_period AS person_income_period, "
        "    pmt.expense AS expense_id, "
        "    exp.description AS expense_description, "
        "    exp.amount AS expense_amount, "
        "    pmt.amount "
        "FROM payment pmt "
        "LEFT JOIN person prsn ON pmt.paid_by = prsn.id "
        "LEFT JOIN expense exp ON pmt.expense = exp.id";
    QSqlQuery q = QSqlQuery(qStr);
    while (q.next()) {
        makePayment(&q);
        qDebug() << "Load payment: " << q.value(0).toString();
    }

    qDebug() << "Payment count: " << m_payments.count();
}

int PaymentModel::rowCount(const QModelIndex &parent) const {
    return m_payments.count();
}

int PaymentModel::columnCount(const QModelIndex &parent) const {
    return Payment::fields.size();
}

QVariant PaymentModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole && !m_payments.isEmpty()) {
        QString field = Payment::fields.at(index.column());

        return m_payments[index.row()]->getData(field, Qt::DisplayRole);
    }

    return QVariant();
}

QVariant PaymentModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return Payment::fields[section];
    }

    return QVariant();
}

Qt::ItemFlags PaymentModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    } else if (index.column() == 0) {
        // ID is always column 0 and should be read-only
        return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool PaymentModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        QString field = Payment::fields.at(index.column());
        
        // TODO: set expense and paidBy on Payment

        m_payments[index.row()]->setData(field, value);
        m_entityManager->update(m_payments[index.row()]);
        
        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

bool PaymentModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row > rowCount() || count <= 0) {
        return false;
    }

    beginInsertRows(parent, row, row + count - 1);
    m_payments.push_back(nullptr);
    endInsertRows();

    return true;
}

bool PaymentModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row > rowCount() || count <= 0) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++) {
        m_payments.removeAt(row);
    }

    endRemoveRows();

    return true;
}

void PaymentModel::addPayment(std::shared_ptr<EntityInterface> payment) {
    // TODO write insert payment query
    QSqlQuery q;
    q.prepare("INSERT INTO payment (description, amount) VALUES (:description, :amount) RETURNING id");
    q.bindValue(":description", payment->getData("description").toString());
    q.bindValue(":amount", payment->getData("amount").toFloat());

    if (q.exec() == false) {
        // TODO: handle error
        qDebug() << "Failed to insert payment: " << payment->getData("description");
        return;
    }
    q.next();
    qDebug() << "Inserted: " << q.value(0).toInt();

    payment->setData("id", q.value(0));

    if (insertRows(rowCount(), 1)) {
        m_payments.replace(rowCount() - 1, payment);
    }
}

void PaymentModel::removePayment(std::shared_ptr<EntityInterface> payment) {
    QSqlQuery q;
    q.prepare("DELETE FROM payment WHERE id = :id");
    q.bindValue(":id", payment->getId());
    if (q.exec() == false) {
        // TODO: handle error
        qDebug() << "Failed to delete payment: " << payment->getId();
        return;
    }

    qDebug() << "Delete payment: " << payment->getId();
    int row = -1;
    for (int i = 0; i < m_payments.size(); i++) {
        if (m_payments.at(i) == payment) {
            row = i;
            break;
        }
    }

    if (row != -1) {
        removeRows(row, 1);
    }
}

std::shared_ptr<EntityInterface> PaymentModel::getRow(int row) {
    if (row < 0 || row > m_payments.size() - 1) {
        return nullptr;
    }

    return m_payments.at(row);
}


/**
 * Query result columns:
 * 
 * 0 pmt.id
 * 1 pmt.paid_by AS person_id
 * 2 prsn.name AS person_name
 * 3 prsn.income AS person_income
 * 4 prsn.income_period AS person_income_period
 * 5 pmt.expense AS expense_id
 * 6 exp.description AS expense_description
 * 7 exp.amount AS expense_amount
 * 8 pmt.amount
 */
void PaymentModel::makePayment(QSqlQuery* q) {
        std::shared_ptr<Payment> pmt = std::make_shared<Payment>(Payment());
        pmt->setData("id", q->value(0).toInt());
        pmt->setData("amount", q->value(8).toFloat());

        std::shared_ptr<Expense> exp = std::make_shared<Expense>(Expense());
        exp->setData("id", q->value(5).toInt());
        exp->setData("description", q->value(6).toString());
        exp->setData("amount", q->value(7).toFloat());
        pmt->setExpense(exp);

        std::shared_ptr<Person> person = std::make_shared<Person>(Person());
        person->setData("id", q->value(1).toInt());
        person->setData("name", q->value(2).toString());
        person->setData("income", q->value(3).toFloat());
        person->setData("income_period", q->value(4).toString());
        pmt->setPaidBy(person);

        m_payments.push_back(pmt);
}
