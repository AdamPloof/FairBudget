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
    }

    QSqlQuery q = QSqlQuery("SELECT id, description, amount FROM expense");
    while (q.next()) {
        int id = q.value(0).toInt();
        if (m_identityMap[EntityType::EXPENSE].contains(id)) {
            // Expense is already managed, update with current value
            m_identityMap[EntityType::EXPENSE][id]->setData("description", q.value(1));
            m_identityMap[EntityType::EXPENSE][id]->setData("amount", q.value(2));
        } else {
            // Expense isn't managed yet
            std::shared_ptr<Expense> e = std::make_shared<Expense>(Expense());
            e->setData("id", q.value(0));
            e->setData("description", q.value(1));
            e->setData("amount", q.value(2));
            m_identityMap[EntityType::EXPENSE][e->getId()] = e;
        }

        qDebug() << "Load expense: " << q.value(0).toString();
    }

    qDebug() << "Expense count: " << m_identityMap[EntityType::EXPENSE].count();
}

void UnitOfWork::fetchIncomePeriods() {
    if (!m_identityMap.contains(EntityType::INCOME_PERIOD)) {
        m_identityMap[EntityType::INCOME_PERIOD] = QHash<int, std::shared_ptr<EntityInterface>>();
    }

    QSqlQuery q = QSqlQuery("SELECT id, period, label FROM income_period");
    while (q.next()) {
        int id = q.value(0).toInt();
        if (m_identityMap[EntityType::INCOME_PERIOD].contains(id)) {
            // IncomePeriod is already managed, update with current value
            m_identityMap[EntityType::INCOME_PERIOD][id]->setData("period", q.value(1));
            m_identityMap[EntityType::INCOME_PERIOD][id]->setData("label", q.value(2));            
        } else {
            // IncomePeriod isn't managed yet
            std::shared_ptr<IncomePeriod> p = std::make_shared<IncomePeriod>(IncomePeriod());
            p->setData("id", q.value(0).toInt());
            p->setData("period", q.value(1));
            p->setData("label", q.value(2));
            m_identityMap[EntityType::INCOME_PERIOD][p->getId()] = p;
        }

        qDebug() << "Load IncomePeriod: " << q.value(0).toString();
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
    while (q.next()) {
        int periodId = q.value(3).toInt();
        std::shared_ptr<IncomePeriod> period;
        if (m_identityMap[EntityType::INCOME_PERIOD].contains(periodId)) {
            // IncomePeriod is already managed
            period = std::dynamic_pointer_cast<IncomePeriod>(
                m_identityMap[EntityType::INCOME_PERIOD].value(periodId)
            );
        } else {
            // IncomePeriod isn't managed yet
            period = std::make_shared<IncomePeriod>(IncomePeriod());
            period->setData("id", q.value(3));
            m_identityMap[EntityType::INCOME_PERIOD][periodId] = period;
        }

        period->setData("period", q.value(4));
        period->setData("label", q.value(5));

        int personId = q.value(0).toInt();
        std::shared_ptr<Person> person;
        if (m_identityMap[EntityType::PERSON].contains(personId)) {
            // Person is already managed
            person = std::dynamic_pointer_cast<Person>(
                m_identityMap[EntityType::PERSON][personId]
            );
        } else {
            // Person is not managed yet
            person = std::make_shared<Person>(Person());
            person->setData("id", q.value(0));
            m_identityMap[EntityType::PERSON][personId] = person;
        }
        person->setData("name", q.value(1));
        person->setData("income", q.value(2));
        person->setIncomePeriod(period);

        qDebug() << "Load person: " << q.value(0).toString();
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
    while (q.next()) {
        int periodId = q.value(4).toInt();
        std::shared_ptr<IncomePeriod> period;
        if (m_identityMap[EntityType::INCOME_PERIOD].contains(periodId)) {
            // IncomePeriod is already managed
            period = std::dynamic_pointer_cast<IncomePeriod>(
                m_identityMap[EntityType::INCOME_PERIOD].value(periodId)
            );
        } else {
            // IncomePeriod isn't managed yet
            period = std::make_shared<IncomePeriod>(IncomePeriod());
            period->setData("id", q.value(4));
            m_identityMap[EntityType::INCOME_PERIOD][periodId] = period;
        }

        period->setData("period", q.value(5));
        period->setData("label", q.value(6));

        int personId = q.value(1).toInt();
        std::shared_ptr<Person> person;
        if (m_identityMap[EntityType::PERSON].contains(personId)) {
            // Person is already managed
            person = std::dynamic_pointer_cast<Person>(
                m_identityMap[EntityType::PERSON][personId]
            );
        } else {
            // Person is not managed yet
            person = std::make_shared<Person>(Person());
            person->setData("id", q.value(1));
            m_identityMap[EntityType::PERSON][personId] = person;
        }

        person->setData("name", q.value(2));
        person->setData("income", q.value(3));
        person->setIncomePeriod(period);

        int expenseId = q.value(7).toInt();
        std::shared_ptr<Expense> expense;
        if (m_identityMap[EntityType::EXPENSE].contains(expenseId)) {
            // Expense already managed, update with current value
            expense = std::dynamic_pointer_cast<Expense>(
                m_identityMap[EntityType::EXPENSE].value(expenseId)
            );
        } else {
            // Expense not managed yet
            expense = std::make_shared<Expense>(Expense());
            expense->setData("id", q.value(7));
            m_identityMap[EntityType::EXPENSE][expenseId] = expense;
        }

        expense->setData("description", q.value(8));
        expense->setData("amount", q.value(9));

        int paymentId = q.value(0).toInt();
        std::shared_ptr<Payment> payment;
        if (m_identityMap[EntityType::PAYMENT].contains(paymentId)) {
            // Payment already managed, update with current value
            payment = std::dynamic_pointer_cast<Payment>(
                m_identityMap[EntityType::PAYMENT].value(paymentId)
            );
        } else {
            // Payment not managed yet
            payment = std::make_shared<Payment>(Payment());
            payment->setData("id", q.value(0));
            m_identityMap[EntityType::PAYMENT][paymentId] = payment;
        }

        payment->setData("amount", q.value(10));
        payment->setPaidBy(person);
        payment->setExpense(expense);

        qDebug() << "Load Payment: " << q.value(0).toString();
    }

    qDebug() << "Payment count: " << m_identityMap[EntityType::PAYMENT].count();
}
