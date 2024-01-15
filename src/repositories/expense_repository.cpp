#include <QAbstractTableModel>
#include <QSqlQuery>

#include "expense_repository.h"
#include "../services/entity_manager.h"
#include "../models/model_interface.h"
#include "../models/expense.h"

ExpenseRepository::ExpenseRepository(std::shared_ptr<EntityManager> em)
    : EntityRepository(em) {}

void ExpenseRepository::fetchRecords(ModelInterface* model) {
    using ExpenseLine = QList<QString>;

    EntityQueryParams params;
    params.entityName = model->name();
    params.fields = model->fields();
    QSqlQuery query = m_entityManager->fetchRecords(params);

    while (query.next()) {
        ExpenseLine row;
        for (QString field : params.fields) {
            QString val = query.value(field).toString();
            row.push_back(val);
            qDebug() << val;
        }

        model->addRow(row);
    }
}
