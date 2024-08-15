#ifndef UNIT_OF_WORK_H
#define UNIT_OF_WORK_H

#include <QHash>
#include <QList>
#include <QString>
#include <QSqlQuery>
#include <memory>
#include <unordered_map>

#include "../entities/entity_interface.h"

class Expense;
class IncomePeriod;
class Person;
class Payment;

/**
 * A very basic implementation of a UnitOfWork. Really more of a faux UnitOfWork. If I eventually
 * decide to make a *real* ORM style manager for handling entities, this would be the place to start.
 */
class UnitOfWork {
public:
    UnitOfWork();

    /**
     * Return a pointer to the entity of the given type and ID.
     * If the entity is not found, then nullptr is returned
     */
    std::shared_ptr<EntityInterface> tryGetById(int id, const EntityType &t);

    /**
     * Return a list of pointers to all managed entities of a given type,
     * if there are no entities of the given type yet in the identityMap, then
     * fetch them from the database.
     */
    QList<std::shared_ptr<EntityInterface>> retrieveAll(const EntityType &t, bool forceFetch = false);

    bool doInsert(const EntityInterface &e);
    bool doUpdate(const EntityInterface &e);
    bool doDelete(const EntityInterface &e);
private:
    using MethodPtr = void (UnitOfWork::*)();
    using PropertyMap = QHash<int, QString>; 

    void fetchExpenses();
    void fetchIncomePeriods();
    void fetchPersons();
    void fetchPayments();
    std::shared_ptr<EntityInterface> makeOrUpdateEntity(
        const EntityType &t,
        int id,
        const PropertyMap &props,
        QSqlQuery &q
    );

    /**
     * m_identityMap is the container for all managed entites in the application.
     * Entities are group by EntityType and then mapped by ID
     */
    QHash<
        EntityType,
        QHash<int, std::shared_ptr<EntityInterface>>
    > m_identityMap;
    std::unordered_map<EntityType, MethodPtr> m_fetchMap;
};

#endif // UNIT_OF_WORK
