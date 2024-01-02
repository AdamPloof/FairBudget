#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>

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
