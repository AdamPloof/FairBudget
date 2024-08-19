#ifndef ENTITY_OPTIONS_LOADER_H
#define ENTITY_OPTIONS_LOADER_H

#include <QHash>
#include <QString>
#include <memory>

class EntityManager;
enum class EntityType;

class EntityOptionsLoader {
public:
    EntityOptionsLoader(std::shared_ptr<EntityManager> em);
    bool fetchOptions(const EntityType &t, QHash<int, QString> &opts) const;

private:
    QString getLabelField(const EntityType &t) const;

    std::shared_ptr<EntityManager> m_entityManager;
};

#endif // ENTITY_OPTIONS_LOADER_H
