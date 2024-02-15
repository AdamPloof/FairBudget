#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <QSqlQuery>
#include <QVariantList>
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

    QSqlQuery fetchRecords(EntityQueryParams params);
    void insertRecords(QString table, std::vector<QString> fields, std::vector<QVariantList> values);
    void runQuery(QString queryStr);

private:
    QString constructQuery(EntityQueryParams& params);
    std::string joinFields(std::vector<QString> fields);
    std::string fieldParams(int fieldCount);
};

#endif // ENTITY_MANAGER_H
