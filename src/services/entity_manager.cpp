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
