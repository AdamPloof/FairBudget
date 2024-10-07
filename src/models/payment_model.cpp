#include <QList>
#include <QString>
#include <QSqlError>
#include <QDebug>

#include "payment_model.h"
#include "../services/entity_manager.h"
#include "../entities/entity_interface.h"
#include "../entities/payment.h"
#include "../entities/income_period.h"
#include "../services/utils.h"

// TODO: prevent adding more payments than expenses and prevent over-paying expenses

using namespace FairBudget::utils;

PaymentModel::PaymentModel(
    std::shared_ptr<EntityManager> em,
    QObject *parent
) : m_entityManager(em) {

}

// PaymentModel::~ExpenseModel() {
    
// }

void PaymentModel::load() {
    if (!m_entityManager->isReady()) {
        throw std::runtime_error("EntityManager must be ready when loading models");
    }

    m_payments = m_entityManager->findAll(EntityType::PAYMENT);
}

void PaymentModel::setLocale(std::shared_ptr<QLocale> locale) {
    m_locale = locale;
}

int PaymentModel::rowCount(const QModelIndex &parent) const {
    return m_payments.count();
}

int PaymentModel::columnCount(const QModelIndex &parent) const {
    return Payment::fields.size();
}

QVariant PaymentModel::data(const QModelIndex &index, int role) const {
    if (m_payments.isEmpty()) {
        return QVariant();
    }

    if (role == Qt::EditRole) {
        QString field = Payment::fields.at(index.column());
        return m_payments[index.row()]->getData(field, role);
    } else if (role == Qt::DisplayRole) {
        QString field = Payment::fields.at(index.column());
        QVariant val = m_payments[index.row()]->getData(field, role);
        if (field == "amount" && m_locale != nullptr) {
            return m_locale->toCurrencyString(val.toDouble());
        }

        return val;
    }

    return QVariant();
}

QVariant PaymentModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return sentenceCase(Payment::fields[section]);
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

/**
 * When setting paidBy and expense fields, the value received will be the id
 * of the entity
 */
bool PaymentModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        QString field = Payment::fields.at(index.column());
        if (field == "paid_by") {
            std::shared_ptr<Payment> p = std::dynamic_pointer_cast<Payment>(m_payments[index.row()]);
            std::shared_ptr<Person> paidBy = m_entityManager->find<Person>(value.toInt());
            if (!paidBy) {
                qDebug() << "No person found for id: " << value.toInt();
                return false;
            }

            p->setPaidBy(paidBy);
        } else if (field == "expense") {
            std::shared_ptr<Payment> p = std::dynamic_pointer_cast<Payment>(m_payments[index.row()]);
            std::shared_ptr<Expense> e = m_entityManager->find<Expense>(value.toInt());
            if (!e) {
                qDebug() << "No expense found for id: " << value.toInt();
                return false;
            }

            p->setExpense(e);
            p->setData("amount", e->getData("amount"));
            // TODO: restrict amount to unpaid amount for selected expense

        } else {
            m_payments[index.row()]->setData(field, value);
        }

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

    emit dataChanged(parent, parent, {Qt::EditRole});

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
    emit dataChanged(parent, parent, {Qt::EditRole});

    return true;
}

void PaymentModel::addPayment(std::shared_ptr<EntityInterface> payment) {
    if (!m_entityManager->persist(payment)) {
        return;
    }

    if (insertRows(rowCount(), 1)) {
        m_payments.replace(rowCount() - 1, payment);
    }
}

void PaymentModel::removePayment(std::shared_ptr<EntityInterface> payment) {
    if (!m_entityManager->remove(payment)) {
        return;
    }

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
