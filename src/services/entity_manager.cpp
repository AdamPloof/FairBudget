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

bool EntityManager::update(std::shared_ptr<EntityInterface> entity) const {
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
        return false;
    }

    return true;
}

/**
 * Returns true on successfully inserting entity into the database.
 */
bool EntityManager::persist(std::shared_ptr<EntityInterface> entity) {
    return m_unitOfWork.insert(entity);
}

bool EntityManager::remove(std::shared_ptr<EntityInterface> entity) {
    QString qStr = "DELETE FROM " + entity->entityName() + " WHERE id = :id";
    QSqlQuery q;
    q.prepare(qStr);
    q.bindValue(":id", entity->getId());
    if (q.exec() == false) {
        qDebug() << "Failed to delete " << entity->entityName() << ": " << entity->getId();
        return false;
    }

    return m_unitOfWork.remove(entity);
}

template <>
std::shared_ptr<Expense> EntityManager::find<Expense>(int id) {
    return std::dynamic_pointer_cast<Expense>(m_unitOfWork.tryGetById(id, EntityType::EXPENSE));
}

template <>
std::shared_ptr<IncomePeriod> EntityManager::find<IncomePeriod>(int id) {
    return std::dynamic_pointer_cast<IncomePeriod>(m_unitOfWork.tryGetById(id, EntityType::INCOME_PERIOD));
}

template <>
std::shared_ptr<Person> EntityManager::find<Person>(int id) {
    return std::dynamic_pointer_cast<Person>(m_unitOfWork.tryGetById(id, EntityType::PERSON));
}

template <>
std::shared_ptr<Payment> EntityManager::find<Payment>(int id) {
    return std::dynamic_pointer_cast<Payment>(m_unitOfWork.tryGetById(id, EntityType::PAYMENT));
}

QList<std::shared_ptr<EntityInterface>> EntityManager::findAll(const EntityType &t, bool forceFetch) {
    return m_unitOfWork.retrieveAll(t, forceFetch);
}

template <>
QList<std::shared_ptr<Expense>> EntityManager::findAll<Expense>(bool forceFetch) {
    return m_unitOfWork.retrieveAll<Expense>(forceFetch);
}

template <>
QList<std::shared_ptr<Person>> EntityManager::findAll<Person>(bool forceFetch) {
    return m_unitOfWork.retrieveAll<Person>(forceFetch);
}

template <>
QList<std::shared_ptr<Payment>> EntityManager::findAll<Payment>(bool forceFetch) {
    return m_unitOfWork.retrieveAll<Payment>(forceFetch);
}

template <>
QList<std::shared_ptr<IncomePeriod>> EntityManager::findAll<IncomePeriod>(bool forceFetch) {
    return m_unitOfWork.retrieveAll<IncomePeriod>(forceFetch);
}
