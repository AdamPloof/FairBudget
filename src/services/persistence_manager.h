#ifndef PERSISTENCE_MANAGER_H
#define PERSISTENCE_MANAGER_H

#include <QObject>
#include <QModelIndex>
#include <QList>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "entity_manager.h"
#include "../models/model_interface.h"

struct DeletedModel {
    std::string modelType;
    int id;
};

typedef QString model_type;

class PersistenceManager : public QObject {
    Q_OBJECT

public:
    explicit PersistenceManager(QObject *parent = nullptr);
    void setEntityManager(std::shared_ptr<EntityManager> em);
    void persist(std::shared_ptr<ModelInterface> model);
    void update(std::shared_ptr<ModelInterface> model);
    void remove(DeletedModel model);
    void flush();

private:
    void execUpdate();

    std::shared_ptr<EntityManager> m_em;
    std::unordered_map<model_type, std::vector<ModelInterface>> m_inserts;
    std::unordered_map<model_type, std::vector<ModelInterface>> m_updates;
    std::unordered_map<model_type, std::vector<DeletedModel>> m_deletes;
};

#endif // PERSISTENCE_MANAGER_H
