#include <QAbstractTableModel>
#include <QSqlQuery>
#include <memory>

#include "person_repository.h"
#include "../services/entity_manager.h"
#include "../models/model_interface.h"
#include "../models/person.h"

PersonRepository::PersonRepository(std::shared_ptr<EntityManager> em)
    : EntityRepository(em) {}

void PersonRepository::fetchRecords(ModelContainer* outContainer) {
    EntityQueryParams params;
    params.entityName = Person::name;
    params.fields = Person::fields;
    QSqlQuery query = m_entityManager->fetchRecords(params);

    while (query.next()) {
        std::shared_ptr<Person> p = std::make_shared<Person>(Person());
        outContainer->push_back(p);
        for (QString field : params.fields) {
            QString val = query.value(field).toString();
            outContainer->back()->setData(field, val);
            // qDebug() << val;
        }
    }
}
