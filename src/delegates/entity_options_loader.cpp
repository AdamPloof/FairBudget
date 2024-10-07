#include <QList>

#include "entity_options_loader.h"
#include "../services/entity_manager.h"
#include "../entities/entity_interface.h"

EntityOptionsLoader::EntityOptionsLoader(std::shared_ptr<EntityManager> em) : m_entityManager(em) {

}

bool EntityOptionsLoader::fetchOptions(const EntityType &t, QHash<int, QString> &opts) const {
    QList<std::shared_ptr<EntityInterface>> entities = m_entityManager->findAll(t, true);
    QString labelField = getLabelField(t);
    for (const auto &e : entities) {
        opts.insert(e->getId(), e->getData(labelField).toString());
    }

    return true;
}

QString EntityOptionsLoader::getLabelField(const EntityType &t) const {
    switch (t) {
        case EntityType::EXPENSE:
            return "description";
        case EntityType::PERSON:
            return "name";
        case EntityType::INCOME_PERIOD:
            return "label";
        default:
            throw std::invalid_argument("Can't get entity options for the entity type provided");
    }
}
