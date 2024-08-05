#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QDebug>
#include <sstream>
#include <cstddef>
#include <vector>

#include "entity_manager.h"
#include "unit_of_work.h"
#include "../entities/entity_interface.h"
#include "../entities/income_period.h"
#include "../entities/expense.h"
#include "../entities/person.h"
#include "../entities/payment.h"

EntityManager::EntityManager() : m_unitOfWork(UnitOfWork()) {}

EntityManager::~EntityManager() {
    EntityManager::closeDb();
}

QSqlDatabase& EntityManager::openDb() {
    static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");

    if (!db.open()) {
        qDebug() << "DB error: " << db.lastError().text() << "\n";
    }

    return db;
}

void EntityManager::closeDb() {
    QSqlDatabase::database().close();
}

bool EntityManager::isReady() const {
    QSqlDatabase db = QSqlDatabase::database();

    return db.isOpen();
}

void EntityManager::update(std::shared_ptr<EntityInterface> entity) const {
    QString qStr = "UPDATE " + entity->entityName() + "\nSET ";
    std::vector<QString> fields = entity->entityFields();

    for (auto field : fields) {
        if (field == "id") {
            continue;
        }

        qStr.append(field + " = :" + field);
        if (field != fields.back()) {
            qStr.append(",\n");
        } else {
            qStr.append("\n");
        }
    }

    qStr.append("WHERE id = " + std::to_string(entity->getId()));
    qDebug() << qStr;

    QSqlQuery q;
    q.prepare(qStr);
    for (auto field : fields) {
        if (field == "id") {
            continue;
        }

        q.bindValue(":" + field, entity->getData(field, Qt::UserRole));
        qDebug() << "Binding:" << field << "with value:" << entity->getData(field, Qt::UserRole);
    }

    if (!q.exec()) {
        qDebug() << "SQL error:" << q.lastError().text();
        qDebug() << "SQL executed:" << qStr;
    }
}

// TODO: add tests for each entity type
template <>
std::shared_ptr<Expense> EntityManager::find<Expense>(int id) const {
    QString qStr = 
        "SELECT id, description, amount "
        "FROM expense "
        "WHERE id = :id";
    QSqlQuery q;
    q.prepare(qStr);
    q.bindValue(":id", id);
    if (!q.exec()) {
        return nullptr;
    }

    if (!q.first()) {
        return nullptr;
    }

    std::shared_ptr<Expense> e = std::make_shared<Expense>(Expense());
    e->setData("id", q.value(0).toInt());
    e->setData("description", q.value(1).toString());
    e->setData("amount", q.value(2).toDouble());

    return e;
}

template <>
std::shared_ptr<Person> EntityManager::find<Person>(int id) const {
    QString qStr = 
        "SELECT p.id, "
        "    p.name, "
        "    p.income, "
        "    i.id AS period_id, "
        "    i.period AS period_desc, "
        "    i.label AS period_label "
        "FROM person p "
        "LEFT JOIN income_period i ON p.income_period = i.id "
        "WHERE p.id = :id";
    QSqlQuery q;
    q.prepare(qStr);
    q.bindValue(":id", id);
    if (!q.exec()) {
        return nullptr;
    }

    if (!q.first()) {
        return nullptr;
    }

    std::shared_ptr<Person> p = std::make_shared<Person>(Person());
    p->setData("id", q.value(0).toInt());
    p->setData("name", q.value(1).toString());
    p->setData("income", q.value(2).toDouble());

    std::shared_ptr<IncomePeriod> i = std::make_shared<IncomePeriod>(IncomePeriod());
    i->setData("id", q.value(3).toInt());
    i->setData("period", q.value(4).toString());
    i->setData("label", q.value(5).toString());
    p->setIncomePeriod(i);

    return p;
}

template <>
std::shared_ptr<Payment> EntityManager::find<Payment>(int id) const {
    QString qStr = 
        "SELECT pay.id, "
        "    pay.paid_by AS person_id, "
        "    per.name AS person_name, "
        "    per.income AS person_income, "
        "    per.income_period AS income_period_id, "
        "    ip.period AS income_period_name, "
        "    ip.label AS income_period_label, "
        "    pay.expense AS expense_id, "
        "    exp.description AS expense_description, "
        "    exp.amount AS expense_amount, "
        "    pay.amount "
        "FROM payment pay "
        "LEFT JOIN person per ON pay.paid_by = per.id "
        "LEFT JOIN income_period ip ON per.income_period = ip.id "
        "LEFT JOIN expense exp ON pay.expense = exp.id "
        "WHERE pay.id = :id";
    QSqlQuery q;
    q.prepare(qStr);
    q.bindValue(":id", id);
    if (!q.exec()) {
        return nullptr;
    }

    if (!q.first()) {
        return nullptr;
    }

    /**
     * Field indices
     * 
     * 0  payment_id
     * 1  person_id
     * 2  person_name
     * 3  person_income
     * 4  income_period_id
     * 5  income_period_name
     * 6  income_period_label
     * 7  expense_id
     * 8  expense_description
     * 9  expense_amount
     * 10 payment_amount
     */

    std::shared_ptr<Payment> payment = std::make_shared<Payment>(Payment());
    payment->setData("id", q.value(0).toInt());
    payment->setData("amount", q.value(10).toDouble());

    std::shared_ptr<IncomePeriod> incomePeriod = std::make_shared<IncomePeriod>(IncomePeriod());
    incomePeriod->setData("id", q.value(4).toInt());
    incomePeriod->setData("period", q.value(5).toString());
    incomePeriod->setData("label", q.value(6).toString());

    std::shared_ptr<Person> person = std::make_shared<Person>(Person());
    person->setData("id", q.value(1).toInt());
    person->setData("name", q.value(2).toString());
    person->setData("income", q.value(3).toDouble());
    person->setIncomePeriod(incomePeriod);
    payment->setPaidBy(person);

    std::shared_ptr<Expense> expense = std::make_shared<Expense>(Expense());
    expense->setData("id", q.value(7).toInt());
    expense->setData("description", q.value(8).toString());
    expense->setData("amount", q.value(9).toDouble());
    payment->setExpense(expense);

    return payment;
}

template <>
std::shared_ptr<IncomePeriod> EntityManager::find<IncomePeriod>(int id) const {
    QString qStr = 
        "SELECT id, period, label "
        "FROM income_period "
        "WHERE id = :id";
    QSqlQuery q;
    q.prepare(qStr);
    q.bindValue(":id", id);
    if (!q.exec()) {
        return nullptr;
    }

    if (!q.first()) {
        return nullptr;
    }

    std::shared_ptr<IncomePeriod> i = std::make_shared<IncomePeriod>(IncomePeriod());
    i->setData("id", q.value(0).toInt());
    i->setData("period", q.value(1).toString());
    i->setData("label", q.value(2).toString());

    return i;
}

QList<std::shared_ptr<EntityInterface>> EntityManager::findAll(const EntityType &t) {
    return m_unitOfWork.getAllByType(t);
}
