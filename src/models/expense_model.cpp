#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <vector>

#include "expense_model.h"
#include "../services/entity_manager.h"
#include "../entities/expense.h"
#include "../entities/entity_interface.h"

ExpenseModel::ExpenseModel(
    std::shared_ptr<EntityManager> em,
    QObject *parent
) : m_entityManager(em) {

}

// ExpenseModel::~ExpenseModel() {
    
// }

void ExpenseModel::load() {
    QSqlQuery q = QSqlQuery("SELECT id, description, amount FROM expense");
    while (q.next()) {
        std::shared_ptr<Expense> exp = std::make_shared<Expense>(Expense());
        exp->setData("id", q.value(0).toInt());
        exp->setData("description", q.value(1).toString());
        exp->setData("amount", q.value(2).toFloat());
        m_expenses.push_back(exp);

        qDebug() << "Load item: " << q.value(0).toString();
    }

    qDebug() << "Expense count: " << m_expenses.count();
}

int ExpenseModel::rowCount(const QModelIndex &parent) const {
    return m_expenses.count();
}

int ExpenseModel::columnCount(const QModelIndex &parent) const {
    return Expense::fields.size();
}

QVariant ExpenseModel::data(const QModelIndex &index, int role) const {
    if (role == Qt::DisplayRole && !m_expenses.isEmpty()) {
        QString field = Expense::fields.at(index.column());

        return m_expenses[index.row()]->getData(field);
    }

    return QVariant();
}

QVariant ExpenseModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return Expense::fields[section];
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

    return true;
}

void ExpenseModel::addExpense(std::shared_ptr<EntityInterface> entity) {
    QSqlQuery q;
    q.prepare("INSERT INTO expense (description, amount) VALUES (:description, :amount) RETURNING id");
    q.bindValue(":description", entity->getData("description").toString());
    q.bindValue(":amount", entity->getData("amount").toFloat());
    q.exec();
    q.next();

    qDebug() << "Inserted: " << q.value(0).toInt();

    entity->setData("id", q.value(0));

    if (insertRows(rowCount(), 1)) {
        m_expenses.replace(rowCount() - 1, entity);
    }
}
