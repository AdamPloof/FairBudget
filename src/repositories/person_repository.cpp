#include <QAbstractTableModel>
#include <QSqlQuery>

#include "person_repository.h"
#include "../services/entity_manager.h"
#include "../models/model_interface.h"
#include "../models/person.h"

PersonRepository::PersonRepository(std::shared_ptr<EntityManager> em)
    : EntityRepository(em) {}

void PersonRepository::fetchRecords(ModelInterface* model) {
    using PersonRow = QList<QString>;

    EntityQueryParams params;
    params.entityName = model->name();
    params.fields = model->fields();
    QSqlQuery query = m_entityManager->fetchRecords(params);

    while (query.next()) {
        PersonRow row;
        for (QString field : params.fields) {
            QString val = query.value(field).toString();
            row.push_back(val);
            // qDebug() << val;
        }

        model->addRow(row);
    }
}
