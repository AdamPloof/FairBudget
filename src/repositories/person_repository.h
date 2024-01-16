#ifndef PERSON_REPOSITORY_H
#define PERSON_REPOSITORY_H

#include <memory>
#include "entity_repository.h"

class EntityManager;

class PersonRepository : public EntityRepository {
public:
    PersonRepository(std::shared_ptr<EntityManager> em);

    void fetchRecords(ModelInterface* model) override;
};

#endif // PERSON_REPOSITORY_H
