#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <QSqlQuery>
#include <QVariantList>
#include <vector>
#include <memory>

class QSqlDatabase;
class QString;
class ModelInterface;

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
    QSqlQuery fetchRecords(const QString queryStr);
    void insertRecords(std::shared_ptr<ModelInterface> model);
    void runQuery(QString queryStr);

private:
    QString constructQuery(EntityQueryParams& params);
    std::string joinFields(std::vector<QString> fields);
    std::string fieldParams(std::vector<QString> fields);
};

#endif // ENTITY_MANAGER_H
