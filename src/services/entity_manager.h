#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <QSqlQuery>
#include <vector>

class QSqlDatabase;
class QString;

struct EntityQueryParams {
    QString entityName;
    std::vector<QString> fields;
};

class EntityManager {

public:
    EntityManager();
    ~EntityManager();

    static QSqlDatabase& openDb();
    static void closeDb();

    QSqlQuery fetchRecords(QString queryStr);
    QSqlQuery fetchRecords(EntityQueryParams params);
    void runQuery(QString queryStr);

private:
    QString constructQuery(EntityQueryParams& params);
};

#endif // ENTITY_MANAGER_H
