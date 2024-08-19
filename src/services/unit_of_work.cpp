#include <QSqlQuery>
#include <QDebug>

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

std::shared_ptr<EntityInterface> UnitOfWork::tryGetById(int id, const EntityType &t) {
    if (!m_identityMap.contains(t)) {
        retrieveAll(t);
    }
    
    if (!m_identityMap[t].contains(id)) {
        return nullptr;
    }

    return m_identityMap[t][id];
}

/**
 * Fetch entities for the given type from the database.
 * 
 * If forceFetch is true and entities have already been fetched from the database, then return all
 * managed entities of the given type. Otherwise (re)fetch from database.
 */
QList<std::shared_ptr<EntityInterface>> UnitOfWork::retrieveAll(const EntityType &t, bool forceFetch) {
    if (m_identityMap.contains(t) && forceFetch == false) {
        return m_identityMap[t].values();
    }

    auto fetch = m_fetchMap.find(t);
    if (fetch == m_fetchMap.end()) {
        throw std::runtime_error("No fetch method found for entity type");
    }

    // Fetch the entities if they haven't been loaded yet.
    (this->*(fetch->second))();

    return m_identityMap[t].values();
}

bool UnitOfWork::insert(std::shared_ptr<EntityInterface> entity) {
    if (entity->getId()) {
        // TODO check if entity is already managed and update if so.
    }

    std::vector<QString> fields = entity->entityFields();
    QString columns = " (";
    QString values = "VALUES (";
    for (auto field : fields) {
        if (field == "id") {
            continue;
        }

        columns.append(field);
        values.append(":" + field);
        if (field != fields.back()) {
            columns.append(", ");
            values.append(", ");
        } else {
            columns.append(") ");
            values.append(") ");
        }
    }

    QString qStr = "INSERT INTO " + entity->entityName();
    qStr.append(columns);
    qStr.append(values);
    qStr.append( "RETURNING id");

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

    if (q.exec() == false) {
        // TODO: handle error
        qDebug() << "Failed to insert entity";
        return false;
    }

    q.next();
    entity->setData("id", q.value(0));
    manageEntity(entity);
    qDebug() << "Inserted entity: " << q.value(0).toInt();

    return true;
}

bool UnitOfWork::remove(std::shared_ptr<EntityInterface> entity) {
    return remove(entity->entityType(), entity->getId());
}

bool UnitOfWork::remove(const EntityType &t, int id) {
    if (!m_identityMap.contains(t)) {
        return false;
    }

    return m_identityMap[t].remove(id);
}

void UnitOfWork::fetchExpenses() {
    if (!m_identityMap.contains(EntityType::EXPENSE)) {
        m_identityMap[EntityType::EXPENSE] = QHash<int, std::shared_ptr<EntityInterface>>();
    }

    QSqlQuery q = QSqlQuery("SELECT id, description, amount FROM expense");
    const PropertyMap props = {
        {1, "description"},
        {2, "amount"},
    };
    while (q.next()) {
        int id = q.value(0).toInt();
        makeOrUpdateEntity(EntityType::EXPENSE, id, props, q);

        qDebug() << "Load expense: " << id;
    }

    qDebug() << "Expense count: " << m_identityMap[EntityType::EXPENSE].count();
}

void UnitOfWork::fetchIncomePeriods() {
    if (!m_identityMap.contains(EntityType::INCOME_PERIOD)) {
        m_identityMap[EntityType::INCOME_PERIOD] = QHash<int, std::shared_ptr<EntityInterface>>();
    }

    QSqlQuery q = QSqlQuery("SELECT id, period, label FROM income_period");
    const PropertyMap props = {
        {1, "period"},
        {2, "label"},
    };
    while (q.next()) {
        int id = q.value(0).toInt();
        makeOrUpdateEntity(EntityType::INCOME_PERIOD, id, props, q);

        qDebug() << "Load IncomePeriod: " << id;
    }

    qDebug() << "IncomePeriod count: " << m_identityMap[EntityType::INCOME_PERIOD].count();
}

void UnitOfWork::fetchPersons() {
    if (!m_identityMap.contains(EntityType::PERSON)) {
        m_identityMap[EntityType::PERSON] = QHash<int, std::shared_ptr<EntityInterface>>();
    }

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
    const PropertyMap personProps = {
        {1, "name"},
        {2, "income"},
    };
    const PropertyMap periodProps = {
        {4, "period"},
        {5, "label"},
    };
    while (q.next()) {
        int personId = q.value(0).toInt();
        std::shared_ptr<Person> person = std::dynamic_pointer_cast<Person>(
            makeOrUpdateEntity(EntityType::PERSON, personId, personProps, q)
        );

        int periodId = q.value(3).toInt();
        std::shared_ptr<IncomePeriod> period = std::dynamic_pointer_cast<IncomePeriod>(
            makeOrUpdateEntity(EntityType::INCOME_PERIOD, periodId, periodProps, q)
        );
        person->setIncomePeriod(period);

        qDebug() << "Load Person: " << personId;
    }

    qDebug() << "Person count: " << m_identityMap[EntityType::PERSON].count();
}

void UnitOfWork::fetchPayments() {
    if (!m_identityMap.contains(EntityType::PAYMENT)) {
        m_identityMap[EntityType::PAYMENT] = QHash<int, std::shared_ptr<EntityInterface>>();
    }

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
    const PropertyMap paymentProps = {
        {10, "amount"},
    };
    const PropertyMap expenseProps = {
        {8, "description"},
        {9, "amount"},
    };
    const PropertyMap personProps = {
        {2, "name"},
        {3, "income"},
    };
    const PropertyMap periodProps = {
        {5, "period"},
        {6, "label"},
    };
    while (q.next()) {
        int paymentId = q.value(0).toInt();
        std::shared_ptr<Payment> payment = std::dynamic_pointer_cast<Payment>(
            makeOrUpdateEntity(EntityType::PAYMENT, paymentId, paymentProps, q)
        );

        int expenseId = q.value(7).toInt();
        std::shared_ptr<Expense> expense = std::dynamic_pointer_cast<Expense>(
            makeOrUpdateEntity(EntityType::EXPENSE, expenseId, expenseProps, q)
        );
        payment->setExpense(expense);

        int personId = q.value(1).toInt();
        std::shared_ptr<Person> person = std::dynamic_pointer_cast<Person>(
            makeOrUpdateEntity(EntityType::PERSON, personId, personProps, q)
        );

        int periodId = q.value(4).toInt();
        std::shared_ptr<IncomePeriod> period = std::dynamic_pointer_cast<IncomePeriod>(
            makeOrUpdateEntity(EntityType::INCOME_PERIOD, periodId, periodProps, q)
        );
        person->setIncomePeriod(period);
        payment->setPaidBy(person);

        qDebug() << "Load Person: " << personId;
    }

    qDebug() << "Payment count: " << m_identityMap[EntityType::PAYMENT].count();
}

void UnitOfWork::manageEntity(std::shared_ptr<EntityInterface> entity) {
    if (!entity->getId()) {
        throw std::invalid_argument("Entity must have an ID in order to be managed");
    }

    if (!m_identityMap.contains(entity->entityType()))     {
        m_identityMap[entity->entityType()] = QHash<int, std::shared_ptr<EntityInterface>>();
    }

    m_identityMap[entity->entityType()][entity->getId()] = entity;
}

std::shared_ptr<EntityInterface> UnitOfWork::makeOrUpdateEntity(
    const EntityType &entityType,
    int id,
    const PropertyMap &props,
    QSqlQuery &q
) {
    std::shared_ptr<EntityInterface> entity = nullptr;
    if (!m_identityMap[entityType].contains(id)) {
        // Entity isn't managed yet
        switch (entityType) {
            case EntityType::EXPENSE:
                entity = std::make_shared<Expense>(Expense());
                break;
            case EntityType::INCOME_PERIOD:
                entity = std::make_shared<IncomePeriod>(IncomePeriod());
                break;
            case EntityType::PERSON:
                entity = std::make_shared<Person>(Person());
                break;
            case EntityType::PAYMENT:
                entity = std::make_shared<Payment>(Payment());
                break;
            default:
                throw std::runtime_error("Cannot make entity: invalid entity type");
        }
        
        entity->setData("id", id);
        manageEntity(entity);
    }

    QHashIterator<int, QString> i(props);
    while(i.hasNext()) {
        i.next();
        m_identityMap[entityType][id]->setData(i.value(), q.value(i.key()));
    }

    return m_identityMap[entityType][id];
}
