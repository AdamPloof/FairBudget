#ifndef PAYMENT_REPOSITORY_H
#define PAYMENT_REPOSITORY_H

#include <memory>
#include "entity_repository.h"

class EntityManager;

class PaymentRepository : public EntityRepository {
public:
    PaymentRepository(std::shared_ptr<EntityManager> em);

    void fetchRecords(ModelContainer* outContainer) override;
};

#endif // PAYMENT_REPOSITORY_H
