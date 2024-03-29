#include <QString>
#include <QDebug>
#include <vector>
#include <QTableView>

#include "app.h"
#include "services/persistence_manager.h"
#include "repositories/entity_repository.h"
#include "repositories/expense_repository.h"
#include "repositories/person_repository.h"
#include "repositories/payment_repository.h"

#include "./ui_app.h"

App::App(std::shared_ptr<EntityManager> em, QWidget *parent)
    : QMainWindow(parent),
    m_entityManager(em),
    m_formatter(nullptr),
    m_expenseTbl(nullptr),
    m_personTbl(nullptr),
    m_paymentTbl(nullptr),
    m_addExpenseForm(nullptr),
    m_persistenceManager(PersistenceManager()),
    ui(new Ui::App)
{
    ui->setupUi(this);
}

App::~App()
{
    delete ui;
    delete m_expenseTbl;
    delete m_personTbl;
}

void App::run() {
    loadDb();
    loadTables();
    loadForms();
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
    expenseRepo.fetchRecords(&expenses);
    m_expenseTbl = new ExpenseTable(&m_persistenceManager);
    for (auto expenseRow : expenses) {
        m_expenseTbl->addRow(expenseRow);
    }

    ui->expenseTbl->setModel(m_expenseTbl);
    formatTable(ui->expenseTbl);
    
    PersonRepository personRepo = PersonRepository(m_entityManager);
    EntityRepository::ModelContainer persons;
    personRepo.fetchRecords(&persons);
    m_personTbl = new PersonTable();
    for (auto personRow : persons) {
        m_personTbl->addRow(personRow);
    }

    ui->personTbl->setModel(m_personTbl);
    formatTable(ui->personTbl);

    PaymentRepository paymentRepo = PaymentRepository(m_entityManager);
    EntityRepository::ModelContainer payments;
    paymentRepo.fetchRecords(&payments);
    m_paymentTbl = new PaymentTable();
    for (auto paymentRow : payments) {
        m_paymentTbl->addRow(paymentRow);
    }

    ui->paymentTbl->setModel(m_paymentTbl);
    formatTable(ui->paymentTbl);
}

void App::loadForms() {
    m_addExpenseForm = new AddExpenseForm(this);
    m_addExpenseForm->setWindowFlag(Qt::Window);
}

void App::formatTable(QTableView* tbl) {
    if (m_formatter == nullptr) {
        return;
    }

    tbl->verticalHeader()->setVisible(false);

    for (int col = 1; col < tbl->model()->columnCount(); col++) {
        tbl->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Stretch);
    }
}

void App::on_addExpenseBtn_clicked() {
    m_addExpenseForm->show();
}

