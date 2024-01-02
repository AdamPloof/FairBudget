#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class QSqlDatabase;

class EntityManager {

public:
    EntityManager();
    ~EntityManager();

    static QSqlDatabase& openDb();
    static void closeDb();
};

#endif // ENTITY_MANAGER_H
