#include <QList>
#include <QSqlError>
#include <QDebug>

#include "expense_model.h"
#include "../services/entity_manager.h"
#include "../entities/expense.h"
#include "../entities/entity_interface.h"
#include "../services/utils.h"

using namespace FairBudget::utils;

ExpenseModel::ExpenseModel(
    std::shared_ptr<EntityManager> em,
    QObject *parent
) : m_entityManager(em) {

}

// ExpenseModel::~ExpenseModel() {
    
// }

void ExpenseModel::load() {
    if (!m_entityManager->isReady()) {
        throw std::runtime_error("EntityManager must be ready when loading models");
    }

    m_expenses = m_entityManager->findAll(EntityType::EXPENSE);
}

void ExpenseModel::setLocale(std::shared_ptr<QLocale> locale) {
    m_locale = locale;
}

int ExpenseModel::rowCount(const QModelIndex &parent) const {
    return m_expenses.count();
}

int ExpenseModel::columnCount(const QModelIndex &parent) const {
    return Expense::fields.size();
}

QVariant ExpenseModel::data(const QModelIndex &index, int role) const {
    if (m_expenses.isEmpty()) {
        return QVariant();
    }

    if (role == Qt::EditRole) {
        QString field = Expense::fields.at(index.column());
        return m_expenses[index.row()]->getData(field);
    } else if (role == Qt::DisplayRole) {
        QString field = Expense::fields.at(index.column());
        QVariant val = m_expenses[index.row()]->getData(field);
        if (field == "amount" && m_locale != nullptr) {
            return m_locale->toCurrencyString(val.toDouble());
        }

        return val;
    }

    return QVariant();
}

QVariant ExpenseModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return sentenceCase(Expense::fields[section]);
    }

    return QVariant();
}

Qt::ItemFlags ExpenseModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    } else if (index.column() == 0) {
        // ID is always column 0 and should be read-only
        return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ExpenseModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        QString field = Expense::fields.at(index.column());
        m_expenses[index.row()]->setData(field, value);
        m_entityManager->update(m_expenses[index.row()]);
        
        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

bool ExpenseModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row > rowCount() || count <= 0) {
        return false;
    }

    beginInsertRows(parent, row, row + count - 1);
    m_expenses.push_back(nullptr);
    endInsertRows();
 
    emit dataChanged(parent, parent, {Qt::EditRole});

    return true;
}

bool ExpenseModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row > rowCount() || count <= 0) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++) {
        m_expenses.removeAt(row);
    }

    endRemoveRows();
    emit dataChanged(parent, parent, {Qt::EditRole});

    return true;
}

void ExpenseModel::addExpense(std::shared_ptr<EntityInterface> expense) {
    if (!m_entityManager->persist(expense)) {
        return;
    }

    if (insertRows(rowCount(), 1)) {
        m_expenses.replace(rowCount() - 1, expense);
    }
}

void ExpenseModel::removeExpense(std::shared_ptr<EntityInterface> expense) {
    if (!m_entityManager->remove(expense)) {
        return;
    }

    QList<int> removedPmtIds = m_entityManager->cascadeRemovePayments(expense);
    if (!removedPmtIds.empty()) {
        emit relatedPaymentsRemoved(removedPmtIds);
    }

    int row = -1;
    for (int i = 0; i < m_expenses.size(); i++) {
        if (m_expenses.at(i) == expense) {
            row = i;
            break;
        }
    }

    if (row != -1) {
        removeRows(row, 1);
    }
}

std::shared_ptr<EntityInterface> ExpenseModel::getRow(int row) {
    if (row < 0 || row > m_expenses.size() - 1) {
        return nullptr;
    }

    return m_expenses.at(row);
}
