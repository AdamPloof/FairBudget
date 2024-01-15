#include "entity_repository.h"
#include "../services/entity_manager.h"

EntityRepository::EntityRepository(std::shared_ptr<EntityManager> em) : m_entityManager(em)
{

}
