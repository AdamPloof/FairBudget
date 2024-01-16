#include <QString>
#include <QDebug>
#include <vector>
#include <QTableView>

#include "app.h"
#include "repositories/expense_repository.h"
#include "repositories/person_repository.h"
#include "models/expense.h"
#include "models/person.h"
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
    m_expense = new Expense();
    expenseRepo.fetchRecords(m_expense);
    ui->expenseTbl->setModel(m_expense);
    formatTable(ui->expenseTbl);

    PersonRepository personRepo = PersonRepository(m_entityManager);
    m_person = new Person();
    personRepo.fetchRecords(m_person);
    ui->personTbl->setModel(m_person);
    formatTable(ui->personTbl);
}

void App::formatTable(QTableView* tbl) {
    if (m_formatter == nullptr) {
        return;
    }

    tbl->verticalHeader()->setVisible(false);
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
