#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QDebug>
#include <sstream>
#include <cstddef>
#include <vector>

#include "entity_manager.h"
#include "../entities/entity_interface.h"

EntityManager::EntityManager() {}

EntityManager::~EntityManager() {
    EntityManager::closeDb();
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

bool EntityManager::isReady() {
    QSqlDatabase db = QSqlDatabase::database();

    return db.isOpen();
}

void EntityManager::update(std::shared_ptr<EntityInterface> entity) {
    QString qStr = "UPDATE " + entity->entityName() + "\nSET ";
    std::vector<QString> fields = entity->entityFields();

    for (auto field : fields) {
        if (field == "id") {
            continue;
        }

        qStr.append(field + " = :" + field);
        if (field != fields.back()) {
            qStr.append(",\n");
        } else {
            qStr.append("\n");
        }
    }

    qStr.append("WHERE id = " + std::to_string(entity->getId()));
    qDebug() << qStr;

    QSqlQuery q;
    q.prepare(qStr);
    for (auto field : fields) {
        if (field == "id") {
            continue;
        }

        q.bindValue(":" + field, entity->getData(field, Qt::UserRole));
        qDebug() << "Binding:" << field << "with value:" << entity->getData(field, Qt::UserRole);
    }

    if (!q.exec()) {
        qDebug() << "SQL error:" << q.lastError().text();
        qDebug() << "SQL executed:" << qStr;
    }
}