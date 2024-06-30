#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <memory>
#include "services/entity_manager.h"
#include "services/table_formatter.h"
#include "forms/add_expense_form.h"
#include "forms/add_person_form.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class App;
}
QT_END_NAMESPACE

class QTableView;
class ExpenseModel;
class PersonModel;
class PaymentModel;
class QItemSelection;

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
    void on_addPersonBtn_clicked();
    void on_removeExpenseBtn_clicked();
    void on_removePersonBtn_clicked();
    void on_expenseSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void on_personSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    void loadTables();
    void connectForms();
    void connectButtons();

    Ui::App* ui;
    AddExpenseForm* m_addExpenseForm;
    AddPersonForm* m_addPersonForm;
    TableFormatter* m_formatter;
    ExpenseModel* m_expenseModel;
    PersonModel* m_personModel;
    PaymentModel* m_paymentModel;

    std::shared_ptr<EntityManager> m_entityManager;
};

#endif // APP_H
