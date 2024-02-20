#include <QList>
#include <QString>
#include <QVariantList>
#include <vector>
#include <sstream>
#include <exception>

#include "../models/expense.h"
#include "../models/person.h"
#include "../models/payment.h"
#include "persistence_manager.h"

PersistenceManager::PersistenceManager() {
    
}

// TODO: Look into adding entity manager in constructor
void PersistenceManager::setEntityManager(std::shared_ptr<EntityManager> em) {
    m_em = em;
}

void PersistenceManager::persist(std::shared_ptr<ModelInterface> model) {
    ModelType mt = model->modelType();
    if (auto inserts = m_inserts.find(mt); inserts != m_inserts.end()) {
        Changeset changes = {{model->getId().toStdString(), model}};
        m_inserts.insert({mt, changes});
    } else {
        m_inserts.at(mt).insert_or_assign(model->getId().toStdString(), model);
    }
}

void PersistenceManager::update(std::shared_ptr<ModelInterface> model) {
    ModelType mt = model->modelType();
    if (auto updates = m_updates.find(mt); updates != m_updates.end()) {
        Changeset changes = {{model->getId().toStdString(), model}};
        m_updates.insert({mt, changes});
    } else {
        m_updates.at(mt).insert_or_assign(model->getId().toStdString(), model);
    }
}

void PersistenceManager::remove(DeletedModel model) {
    if (auto deletes = m_deletes.find(model.modelType); deletes != m_deletes.end()) {
        DeleteQueue deletedModels = {{model.id, model}};
        m_deletes.insert({model.modelType, deletedModels});
    } else {
        m_deletes.at(model.modelType).insert_or_assign(model.id, model);
    }
}

void PersistenceManager::flush() {
    
}

void PersistenceManager::insertRecords(Changeset changeset) {
    // TODO: insert multiple model instances in single query.
    for (auto [id, model] : changeset) {
        m_em->insertRecords(model);
    }
}

bool PersistenceManager::isNumber(QString s) {
    char* p;
    strtod(s.toStdString().c_str(), &p);
    if (*p) {
        return false;
    }

    return true;
}
