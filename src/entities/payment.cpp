#include "payment.h"

QString Payment::name = "payment";
std::vector<QString> Payment::fields = {"id", "paid_by", "expense", "amount"};

Payment::Payment() {}

QString Payment::entityName() const {
    return Payment::name;
}

EntityType Payment::entityType() const {
    return EntityType::PAYMENT;
}

std::vector<QString> Payment::entityFields() const {
    return Payment::fields;
}

// TODO: handle setting expense and paidBy with ID
void Payment::setData(QString field, QVariant val) {
    if (field == "id") {
        m_id = val.toInt();
    } else if (field == "amount") {
        m_amount = val.toDouble();
    } else {
        throw std::invalid_argument("Invalid field for Payment");
    }
}

void Payment::setPaidBy(std::shared_ptr<Person> person) {
    m_paidBy = person;
}

void Payment::setExpense(std::shared_ptr<Expense> expense) {
    m_expense = expense;
}

const int Payment::getId() const {
    return m_id;
}

QHash<QString, QVariant> Payment::getData(int role) const {
    if (role == Qt::DisplayRole) {
        return {
            {"id", m_id},
            {"paid_by", m_paidBy->getData("name")},
            {"expense", m_expense->getData("description")},
            {"amount", m_amount}
        };
    } else if (role == Qt::UserRole) {
        return {
            {"id", m_id},
            {"paid_by", m_paidBy->getData("id")},
            {"expense", m_expense->getData("id")},
            {"amount", m_amount}
        };
    }

}

QVariant Payment::getData(QString field, int role) const {
    QVariant data;
    if (field == "id") {
        data = m_id;
    } else if (field == "paid_by") {
        if (role == Qt::DisplayRole) {
            data = m_paidBy->getData("name");
        } else if (role == Qt::UserRole) {
            data = m_paidBy->getData("id");
        } else {
            throw std::invalid_argument("Invalid role for Payment data");
        }
    } else if (field == "expense") {
        if (role == Qt::DisplayRole) {
            data = m_expense->getData("description");
        } else if (role == Qt::UserRole) {
            data = m_expense->getData("id");
        } else {
            throw std::invalid_argument("Invalid role for Payment data");
        }
    } else if (field == "amount") {
        data = m_amount;
    } else {
        throw std::invalid_argument("Invalid field for Payment");
    }

    return data;
}

std::shared_ptr<Person> Payment::getPaidBy() const {
    return m_paidBy;
}

std::shared_ptr<Expense> Payment::getExpense() const {
    return m_expense;
}
