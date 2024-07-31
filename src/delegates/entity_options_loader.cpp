#include <QSqlQuery>
#include "entity_options_loader.h"

bool EntityOptionsLoader::fetchOptions(EntityType t, QHash<int, QString> &opts) const {
    QSqlQuery q;
    q.prepare(getEntityQuery(t));
    if (!q.exec()) {
        qDebug() << "Could not fetch options for entity";
        return false;
    }

    while (q.next()) {
        opts.insert(q.value(0).toInt(), q.value(1).toString());
    }

    return true;
}

QString EntityOptionsLoader::getEntityQuery(EntityType t) const {
    QString q;
    switch (t) {
        case EntityType::EXPENSE:
            q = "SELECT id, description FROM expense";
            break;
        case EntityType::INCOME_PERIOD:
            q = "SELECT id, label FROM income_period";
            break;
        case EntityType::PERSON:
            q = "SELECT id, name FROM person";
            break;
        default:
            throw std::invalid_argument("Unable to create options list for entity provided");
    }

    return q;
}
