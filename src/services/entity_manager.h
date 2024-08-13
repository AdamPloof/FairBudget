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

    template <typename T>
    std::shared_ptr<T> find(int id) {
        throw std::runtime_error("Unsupported type for find");
    }

    QList<std::shared_ptr<EntityInterface>> findAll(const EntityType &t);

private:
    UnitOfWork m_unitOfWork;
};

#endif // ENTITY_MANAGER_H
