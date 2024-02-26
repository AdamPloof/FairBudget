#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <memory>
#include "services/entity_manager.h"
#include "services/persistence_manager.h"
#include "services/table_formatter.h"
#include "widgets/expense_table.h"
#include "widgets/person_table.h"
#include "widgets/payment_table.h"
#include "forms/add_expense_form.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class App;
}
QT_END_NAMESPACE

class QTableView;

class App : public QMainWindow
{
    Q_OBJECT

public:
    App(std::shared_ptr<EntityManager> em, QWidget *parent = nullptr);
    ~App();

    void run();
    void setFormatter(TableFormatter* formatter);
    void loadDb();

private slots:
    void on_addExpenseBtn_clicked();

private:
    void loadTables();
    void loadForms();
    void formatTable(QTableView* tbl);

    Ui::App *ui;
    TableFormatter* m_formatter;
    ExpenseTable* m_expenseTbl;
    PersonTable* m_personTbl;
    PaymentTable* m_paymentTbl;
    AddExpenseForm* m_addExpenseForm;

    std::shared_ptr<EntityManager> m_entityManager;
    PersistenceManager m_persistenceManager;
};
#endif // APP_H
