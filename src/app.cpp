#include <QString>
#include <QDebug>
#include <vector>
#include <QTableView>

#include "app.h"
#include "repositories/entity_repository.h"
#include "repositories/expense_repository.h"
#include "repositories/person_repository.h"
#include "models/person.h"

#include "widgets/expense_table.h"
#include "widgets/person_table.h"
#include "./ui_app.h"

App::App(std::shared_ptr<EntityManager> em, QWidget *parent)
    : QMainWindow(parent),
    m_entityManager(em),
    m_formatter(nullptr),
    ui(new Ui::App)
{
    ui->setupUi(this);
}

App::~App()
{
    delete ui;
    delete m_expense;
    delete m_person;
}

void App::run() {
    loadDb();
    loadTables();
}

void App::loadDb() {
    m_entityManager->openDb();
}

void App::setFormatter(TableFormatter* formatter) {
    m_formatter = formatter;
}

void App::loadTables() {
    ExpenseRepository expenseRepo = ExpenseRepository(m_entityManager);
    EntityRepository::ModelContainer expenses;

    // TODO: having to create a dummy model is silly. Fix this
    Expense exp = Expense();
    expenseRepo.fetchRecords(&exp, &expenses);

    m_expense = new ExpenseTable();
    for (auto expenseRow : expenses) {
        m_expense->addRow(expenseRow);
    }

    ui->expenseTbl->setModel(m_expense);
    formatTable(ui->expenseTbl);
    
    // PersonRepository personRepo = PersonRepository(m_entityManager);
    // EntityRepository::ModelContainer persons;

    // // TODO: having to create a dummy model is silly. Fix this
    // Person person = Person();
    // personRepo.fetchRecords(&person, &persons);

    // m_person = new PersonTable();
    // for (auto personRow : persons) {
    //     m_person->addRow(personRow);
    // }

    // ui->personTbl->setModel(m_person);
    // formatTable(ui->personTbl);
}

void App::formatTable(QTableView* tbl) {
    if (m_formatter == nullptr) {
        return;
    }

    tbl->verticalHeader()->setVisible(false);
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
