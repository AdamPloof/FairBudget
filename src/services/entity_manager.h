#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <QSqlQuery>
#include <QVariantList>
#include <vector>
#include <memory>

class QSqlDatabase;
class QString;
class ModelInterface;
class EntityInterface;

class EntityManager {

public:
    EntityManager();
    ~EntityManager();

    static QSqlDatabase& openDb();
    static void closeDb();

    void update(std::shared_ptr<EntityInterface> entity);

    bool isReady();
};

#endif // ENTITY_MANAGER_H
