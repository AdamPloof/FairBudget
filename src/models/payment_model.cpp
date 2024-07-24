#include <QList>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "payment_model.h"
#include "../services/entity_manager.h"
#include "../entities/entity_interface.h"
#include "../entities/payment.h"
#include "../entities/income_period.h"

// TODO: prevent adding more payments than expenses and prevent over-paying expenses

PaymentModel::PaymentModel(
    std::shared_ptr<EntityManager> em,
    QObject *parent
) : m_entityManager(em) {

}

// PaymentModel::~ExpenseModel() {
    
// }

// TODO: payments don't appear to be loading...
void PaymentModel::load() {
    QString qStr =
        "SELECT pmt.id, "
        "    pmt.paid_by AS person_id, "
        "    prsn.name AS person_name, "
        "    prsn.income AS person_income, "
        "    prsn.income_period AS ip_id, "
        "    ip.period AS ip_period, "
        "    ip.label AS ip_label, "
        "    pmt.expense AS expense_id, "
        "    exp.description AS expense_description, "
        "    exp.amount AS expense_amount, "
        "    pmt.amount "
        "FROM payment pmt "
        "LEFT JOIN person prsn ON pmt.paid_by = prsn.id "
        "LEFT JOIN expense exp ON pmt.expense = exp.id "
        "LEFT JOIN income_period ip ON prsn.income_period = ip.id";
    QSqlQuery q = QSqlQuery(qStr);
    while (q.next()) {
        buildPayment(&q);
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

    if (role == Qt::EditRole && !m_payments.isEmpty()) {
        QString field = Payment::fields.at(index.column());

        return m_payments[index.row()]->getData(field, Qt::EditRole);
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
    QSqlQuery q;
    q.prepare("INSERT INTO payment (paid_by, expense, amount) VALUES (:paid_by, :expense, :amount) RETURNING id");
    q.bindValue(":paid_by", payment->getData("paid_by", Qt::UserRole));
    q.bindValue(":expense", payment->getData("expense", Qt::UserRole));
    q.bindValue(":amount", payment->getData("amount", Qt::UserRole));

    if (q.exec() == false) {
        // TODO: handle error
        qDebug() << "Failed to insert payment: " << payment->getData("description");
        return;
    }
    q.next();
    qDebug() << "Inserted payment: " << q.value(0).toInt();

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
 * 0  pmt.id
 * 1  person_id
 * 2  person_name
 * 3  person_income
 * 4  ip_id
 * 5  ip_period
 * 6  ip_label
 * 7  expense_id
 * 8  expense_description
 * 9  expense_amount
 * 10 pmt.amount
 */
void PaymentModel::buildPayment(QSqlQuery* q) {
        std::shared_ptr<Payment> pmt = std::make_shared<Payment>(Payment());
        pmt->setData("id", q->value(0).toInt());
        pmt->setData("amount", q->value(10).toDouble());

        std::shared_ptr<Expense> exp = std::make_shared<Expense>(Expense());
        exp->setData("id", q->value(7).toInt());
        exp->setData("description", q->value(8).toString());
        exp->setData("amount", q->value(9).toDouble());
        pmt->setExpense(exp);

        std::shared_ptr<Person> person = std::make_shared<Person>(Person());
        person->setData("id", q->value(1).toInt());
        person->setData("name", q->value(2).toString());
        person->setData("income", q->value(3).toDouble());

        std::shared_ptr<IncomePeriod> ip = std::make_shared<IncomePeriod>(IncomePeriod());
        ip->setData("id", q->value(4).toInt());
        ip->setData("period", q->value(5).toString());
        ip->setData("label", q->value(6).toString());

        person->setIncomePeriod(ip);
        pmt->setPaidBy(person);

        m_payments.push_back(pmt);
}
