#include <QString>
#include <QDebug>
#include <vector>

#include "app.h"
#include "repositories/expense_repository.h"
#include "models/expense.h"
#include "./ui_app.h"

App::App(std::shared_ptr<EntityManager> em, QWidget *parent)
    : QMainWindow(parent),
    m_entityManager(em),
    ui(new Ui::App)
{
    ui->setupUi(this);
}

App::~App()
{
    delete ui;
    delete m_expense;
}

void App::run() {
    loadDb();
    ExpenseRepository expenseRepo = ExpenseRepository(m_entityManager);
    m_expense = new Expense();
    expenseRepo.fetchRecords(m_expense);
    ui->expenseTbl->verticalHeader()->setVisible(false);
    ui->expenseTbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->expenseTbl->setModel(m_expense);
}

void App::loadDb() {
    m_entityManager->openDb();
}
