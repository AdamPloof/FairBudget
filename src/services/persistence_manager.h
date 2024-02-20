#ifndef PERSISTENCE_MANAGER_H
#define PERSISTENCE_MANAGER_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <sstream>
#include <QString>

#include "entity_manager.h"
#include "../models/model_interface.h"

struct DeletedModel {
    ModelType modelType;
    std::string id;
};

typedef std::unordered_map<std::string, std::shared_ptr<ModelInterface>> Changeset;
typedef std::unordered_map<std::string, DeletedModel> DeleteQueue;

class PersistenceManager : public QObject {

public:
    explicit PersistenceManager();
    void setEntityManager(std::shared_ptr<EntityManager> em);
    void persist(std::shared_ptr<ModelInterface> model);
    void update(std::shared_ptr<ModelInterface> model);
    void remove(DeletedModel model);
    void flush();

private:
    void insertRecords(Changeset changeset);
    void updateRecords(ModelType mt, Changeset changeset);
    void deleteRecords(ModelType mt, DeleteQueue deleteQueue);
    bool isNumber(QString s);

    std::shared_ptr<EntityManager> m_em;
    std::unordered_map<ModelType, Changeset> m_inserts;
    std::unordered_map<ModelType, Changeset> m_updates;
    std::unordered_map<ModelType, DeleteQueue> m_deletes;
};

#endif // PERSISTENCE_MANAGER_H
