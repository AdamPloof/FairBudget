#include "entity_repository.h"

EntityRepository::EntityRepository(std::shared_ptr<EntityManager> em) : 
    m_entityManager(em) {}
