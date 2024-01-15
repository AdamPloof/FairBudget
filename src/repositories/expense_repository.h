#ifndef EXPENSE_REPOSITORY_H
#define EXPENSE_REPOSITORY_H

#include <memory>
#include "entity_repository.h"

class EntityManager;

class ExpenseRepository : public EntityRepository {
public:
    ExpenseRepository(std::shared_ptr<EntityManager> em);

    void fetchRecords(ModelInterface* model) override;
};

#endif // EXPENSE_REPOSITORY_H
