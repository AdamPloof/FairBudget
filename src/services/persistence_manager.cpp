#include <QList>
#include <QString>
#include <vector>

#include "persistence_manager.h"

PersistenceManager::PersistenceManager(QObject *parent) {
    
}

void PersistenceManager::setEntityManager(std::shared_ptr<EntityManager> em) {
    m_em = em;
}

void PersistenceManager::persist(std::shared_ptr<ModelInterface> model) {


}

void PersistenceManager::update(std::shared_ptr<ModelInterface> model) {

}

void PersistenceManager::remove(DeletedModel model) {

}

void PersistenceManager::flush() {
    
}

void PersistenceManager::execUpdate() {
    
}
