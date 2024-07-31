#ifndef ENTITY_OPTIONS_LOADER_H
#define ENTITY_OPTIONS_LOADER_H

#include <QHash>
#include <QString>

#include "../entities/entity_interface.h"

class EntityOptionsLoader {
public:
    bool fetchOptions(EntityType t, QHash<int, QString> &opts) const;

private:
    QString getEntityQuery(EntityType t) const;
};

#endif // ENTITY_OPTIONS_LOADER_H
