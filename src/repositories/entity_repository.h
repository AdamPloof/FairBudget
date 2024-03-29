#ifndef ENTITY_REPOSITORY_H
#define ENTITY_REPOSITORY_H

#include <memory>
#include <vector>
#include "../services/entity_manager.h"

class EntityManager;
class QAbstractTableModel;
class ModelInterface;

class EntityRepository {
public:
    typedef std::vector<std::shared_ptr<ModelInterface>> ModelContainer;

    EntityRepository(std::shared_ptr<EntityManager> em);
    // ~EntityManager();

    virtual void fetchRecords(ModelContainer* outContainer) = 0;

protected:
    std::shared_ptr<EntityManager> m_entityManager;
};

#endif // ENTITY_REPOSTIROY_H
