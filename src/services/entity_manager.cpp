#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QDebug>
#include <sstream>
#include <cstddef>

#include "entity_manager.h"
#include "../models/model_interface.h"

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

QSqlQuery EntityManager::fetchRecords(const QString queryStr) {
    QSqlQuery query = QSqlQuery(queryStr);

    return query;
}

void EntityManager::insertRecords(std::shared_ptr<ModelInterface> model) {
    std::stringstream query;
    query << "INSERT " << " INTO " << model->modelName().toStdString();
    query << ' ' << joinFields(model->modelFields()) << "\nVALUES\n";
    query << fieldParams(model->modelFields());
    const QString queryStr(query.str().c_str());

    QSqlQuery q;
    q.prepare(queryStr);

    for (auto fieldValues : model->getData()) {
        q.bindValue(":" + fieldValues.field, fieldValues.value);
    }
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
    modelFields << '(';
    for (QString field : fields) {
        modelFields << field.toStdString();

        if (field != fields.back()) {
            modelFields << ", ";
        }
    }

    modelFields << ')';

    return modelFields.str();
}

std::string EntityManager::fieldParams(std::vector<QString> fields) {
    std::stringstream params;
    params << '(';
    for (auto field : fields) {
        params << ":" + field.toStdString();
        if (field != fields.back()) {
            params << ", ";
        }
    }

    params << ')';

    return params.str();
}
