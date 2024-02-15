#include <QSqlError>
#include <QSqlDatabase>
#include <QString>
#include <QDebug>
#include <sstream>
#include <cstddef>

#include "entity_manager.h"

EntityManager::EntityManager() {}

EntityManager::~EntityManager() {
    closeDb();
}

QSqlDatabase& EntityManager::openDb() {
    static QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.db");

    if (!db.open()) {
        qDebug() << "DB error: " << db.lastError().text() << "\n";
    }

    return db;
}

void EntityManager::closeDb() {
    QSqlDatabase::database().close();
}

QSqlQuery EntityManager::fetchRecords(EntityQueryParams params) {
    QSqlQuery query = QSqlQuery(constructQuery(params));
    // qDebug() << "Fetching record: " << params.entityName;

    return query;
}

void EntityManager::insertRecords(
    QString table,
    std::vector<QString> fields,
    std::vector<QVariantList> values
) {
    std::stringstream query;
    query << "INSERT " << joinFields(fields);
    query << " INTO " << table.toStdString() << "\nVALUES\n";
    query << fieldParams(fields.size());
}

void EntityManager::runQuery(QString queryStr) {
    
}

QString EntityManager::constructQuery(EntityQueryParams& params) {
    std::stringstream q;
    q <<  "SELECT ";

    std::ptrdiff_t fieldCnt = params.fields.size();
    std::ptrdiff_t currentIdx = 0;
    for (QString field : params.fields) {
        q << field.toStdString();

        if (currentIdx + 1 < fieldCnt) {
            q << ',';
        }

        q << ' ';
        currentIdx++;
    }

    q << "FROM " << params.entityName.toStdString() << ';';

    return QString(q.str().c_str());
}

std::string EntityManager::joinFields(std::vector<QString> fields) {
    std::stringstream modelFields;
    for (QString field : fields) {
        modelFields << field.toStdString();

        if (field != fields.back()) {
            modelFields << ", ";
        }
    }

    return modelFields.str();
}

std::string EntityManager::fieldParams(int fieldCount) {
    std::stringstream params;
    params << '(';
    for (int i = 0; i < fieldCount; i++) {
        params << (i != fieldCount - 1 ? "?, " : "?");
    }

    params << ')';

    return params.str();
}
