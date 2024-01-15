#ifndef ENTITY_REPOSITORY_H
#define ENTITY_REPOSITORY_H

#include <memory>

class EntityManager;
class QAbstractTableModel;
class ModelInterface;

class EntityRepository {
public:
    EntityRepository(std::shared_ptr<EntityManager> em);
    // ~EntityManager();

    virtual void fetchRecords(ModelInterface* model) = 0;

protected:
    std::shared_ptr<EntityManager> m_entityManager;
};

#endif // ENTITY_REPOSTIROY_H
