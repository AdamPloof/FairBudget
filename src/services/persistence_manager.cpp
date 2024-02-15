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

void PersistenceManager::insertRecords(ModelType mt, Changeset changeset) {
    QString table;
    std::vector<QString> fields;
    switch (mt) {
        case ModelType::EXPENSE:
            table = Expense::name;
            fields = Expense::fields;
            break;
        case ModelType::PERSON:
            table = Person::name;
            fields = Person::fields;
            break;
        case ModelType::PAYMENT:
            table = Payment::name;
            fields = Payment::fields;
            break;
        default:
            throw std::invalid_argument("Unknown ModelType provided for insert");
    }

    // TODO: construct values list from changeset
    std::vector<QVariantList> values;

    m_em->insertRecords(table, fields, values);
}

std::string PersistenceManager::modelValues(std::shared_ptr<ModelInterface> model) {
    std::stringstream values;
    QList<QString> data = model->getData();
    for (auto val : data) {
        values << '(';
        if (isNumber(val)) {
            values << val.toStdString();
        } else {
            values << "'" << val.toStdString() << "'";
        }

        if (val != data.last()) {
            values << ", ";
        } else {
            values << ")\n";
        }
    }

    return values.str();
}

bool PersistenceManager::isNumber(QString s) {
    char* p;
    strtod(s.toStdString().c_str(), &p);
    if (*p) {
        return false;
    }

    return true;
}
