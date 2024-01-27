#include <QAbstractTableModel>
#include <QSqlQuery>

#include "expense_repository.h"
#include "../services/entity_manager.h"
#include "../models/model_interface.h"
#include "../models/expense.h"

ExpenseRepository::ExpenseRepository(std::shared_ptr<EntityManager> em)
    : EntityRepository(em) {}

void ExpenseRepository::fetchRecords(ModelContainer* outContainer) {
    EntityQueryParams params;
    params.entityName = Expense::name;
    params.fields = Expense::fields;
    QSqlQuery query = m_entityManager->fetchRecords(params);

    while (query.next()) {
        std::shared_ptr<Expense> e = std::make_shared<Expense>(Expense());
        outContainer->push_back(e);
        for (QString field : params.fields) {
            QString val = query.value(field).toString();
            outContainer->back()->setData(field, val);
            // qDebug() << val;
        }
    }
}
