#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <QSqlQuery>
#include <QList>
#include <QVariantList>
#include <vector>
#include <memory>

#include "unit_of_work.h"
#include "../entities/entity_interface.h"

class QSqlDatabase;
class QString;
class ModelInterface;

class EntityManager {

public:
    EntityManager();
    ~EntityManager();

    static QSqlDatabase& openDb();
    static void closeDb();

    bool isReady() const;
    bool update(std::shared_ptr<EntityInterface> entity) const;
    bool persist(std::shared_ptr<EntityInterface> entity);
    bool remove(std::shared_ptr<EntityInterface> entity);
    QList<std::shared_ptr<EntityInterface>> findAll(const EntityType &t, bool forceFetch = false);

    /**
     * Remove payments related to Expenses and Persons when those entities are removed.
     * Return a list of IDs of any payments that were removed.
     */
    QList<int> cascadeRemovePayments(std::shared_ptr<EntityInterface> entity);

    template <typename T>
    std::shared_ptr<T> find(int id) {
        throw std::runtime_error("Unsupported type for find");
    }

    template <typename T>
    QList<std::shared_ptr<T>> findAll(bool forceFetch = false) {
        throw std::runtime_error("Unsupported type for findAll");
    };

private:
    UnitOfWork m_unitOfWork;
};

#endif // ENTITY_MANAGER_H
