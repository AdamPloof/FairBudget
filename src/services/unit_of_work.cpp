#include <QSqlQuery>

#include "unit_of_work.h"
#include "../entities/expense.h"
#include "../entities/income_period.h"
#include "../entities/person.h"
#include "../entities/payment.h"

UnitOfWork::UnitOfWork() : m_fetchMap({
    {EntityType::EXPENSE, &UnitOfWork::fetchExpenses},
    {EntityType::INCOME_PERIOD, &UnitOfWork::fetchIncomePeriods},
    {EntityType::PERSON, &UnitOfWork::fetchPersons},
    {EntityType::PAYMENT, &UnitOfWork::fetchPayments},
}) {}

const std::shared_ptr<EntityInterface> UnitOfWork::tryGetById(int id, const EntityType &t) {
    loadEntities(t);
    if (!m_identityMap[t].contains(id)) {
        return nullptr;
    }

    return m_identityMap[t][id];
}

/**
 * There are a number of challenges with this implementation. Mainly:
 *  - when should we re-fetch entities?
 *  - what if something external modifies the database outside of the app? How would we know?
 */
QList<std::shared_ptr<EntityInterface>> UnitOfWork::getAllByType(const EntityType &t) {
    loadEntities(t);

    return m_identityMap[t].values();
}

bool UnitOfWork::doInsert(const EntityInterface &e) {
    return false;
}

bool UnitOfWork::doUpdate(const EntityInterface &e) {
    return false;
}

bool UnitOfWork::doDelete(const EntityInterface &e) {
    return false;
}

/**
 * Fetch entities if they haven't been loaded yet. Otherwise return
 * immediately.
 */
void UnitOfWork::loadEntities(const EntityType &t) {
    if (m_identityMap.contains(t)) {
        return;
    }

    auto fetch = m_fetchMap.find(t);
    if (fetch == m_fetchMap.end()) {
        throw std::runtime_error("No fetch method found for entity type");
    }

    // Fetch the entities if they haven't been loaded yet.
    (this->*(fetch->second))();
}

void UnitOfWork::fetchExpenses() {
    if (!m_identityMap.contains(EntityType::EXPENSE)) {
        m_identityMap[EntityType::EXPENSE] = QHash<int, std::shared_ptr<EntityInterface>>();
    } else {
        m_identityMap[EntityType::EXPENSE].clear();
    }

    QSqlQuery q = QSqlQuery("SELECT id, description, amount FROM expense");
    while (q.next()) {
        std::shared_ptr<Expense> e = std::make_shared<Expense>(Expense());
        e->setData("id", q.value(0).toInt());
        e->setData("description", q.value(1).toString());
        e->setData("amount", q.value(2).toDouble());
        m_identityMap[EntityType::EXPENSE][e->getId()] = e;

        qDebug() << "Load expense: " << q.value(0).toString();
    }

    qDebug() << "Expense count: " << m_identityMap[EntityType::EXPENSE].count();
}

void UnitOfWork::fetchIncomePeriods() {
    if (!m_identityMap.contains(EntityType::INCOME_PERIOD)) {
        m_identityMap[EntityType::INCOME_PERIOD] = QHash<int, std::shared_ptr<EntityInterface>>();
    } else {
        m_identityMap[EntityType::INCOME_PERIOD].clear();
    }

    QSqlQuery q = QSqlQuery("SELECT id, period, label FROM income_period");
    while (q.next()) {
        std::shared_ptr<IncomePeriod> p = std::make_shared<IncomePeriod>(IncomePeriod());
        p->setData("id", q.value(0).toInt());
        p->setData("period", q.value(1).toString());
        p->setData("label", q.value(2).toDouble());
        m_identityMap[EntityType::INCOME_PERIOD][p->getId()] = p;

        qDebug() << "Load IncomePeriod: " << q.value(0).toString();
    }

    qDebug() << "IncomePeriod count: " << m_identityMap[EntityType::INCOME_PERIOD].count();
}

void UnitOfWork::fetchPersons() {
    QList<std::shared_ptr<Person>> persons;
    QString qStr = 
        "SELECT p.id, "
        "    p.name, "
        "    p.income, "
        "    i.id AS period_id, "
        "    i.period AS period_desc, "
        "    i.label AS period_label "
        "FROM person p "
        "LEFT JOIN income_period i ON p.income_period = i.id";
    QSqlQuery q = QSqlQuery(qStr);
    while (q.next()) {
        // buildPerson(&q);
        qDebug() << "Load person: " << q.value(0).toString();
    }

    qDebug() << "Person count: " << persons.count();
}

void UnitOfWork::fetchPayments() {

}
