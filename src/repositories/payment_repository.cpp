#include <QAbstractTableModel>
#include <QSqlQuery>

#include "payment_repository.h"
#include "../services/entity_manager.h"
#include "../models/model_interface.h"
#include "../models/payment.h"

PaymentRepository::PaymentRepository(std::shared_ptr<EntityManager> em)
    : EntityRepository(em) {}

// It would be cool to do some flexible way of constructing the query
// from fields and figuring out how to join tables based on some kind of
// model map but... let's just keep it easy and write the query by hand. 
void PaymentRepository::fetchRecords(ModelContainer* outContainer) {
    const char* q = R"(SELECT
                        pay.id,
                        e.description AS 'expense',
                        p.name AS 'paid_by',
                        pay.amount
                    FROM payment pay
                    LEFT JOIN person p ON p.id = pay.paid_by
                    LEFT JOIN expense e ON e.id = pay.expense;)";
    const QString queryStr = QString(q);
    QSqlQuery query = m_entityManager->fetchRecords(queryStr);

    while (query.next()) {
        std::shared_ptr<Payment> e = std::make_shared<Payment>(Payment());
        outContainer->push_back(e);
        for (QString field : Payment::fields) {
            QString val = query.value(field).toString();
            outContainer->back()->setData(field, val);
            // qDebug() << val;
        }
    }
}
