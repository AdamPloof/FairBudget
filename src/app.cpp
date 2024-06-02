#include <QString>
#include <QDebug>
#include <vector>
#include <QTableView>

#include "app.h"
#include "./ui_app.h"
#include "models/expense_model.h"

App::App(std::shared_ptr<EntityManager> em, QWidget *parent)
    : QMainWindow(parent),
    m_entityManager(em),
    ui(new Ui::App)
{
    ui->setupUi(this);

    m_addExpenseForm = new AddExpenseForm(this);
    m_addExpenseForm->setWindowFlag(Qt::Window);
    m_expenseModel = new ExpenseModel(em);
}

App::~App()
{
    delete ui;
    delete m_addExpenseForm;
    delete m_expenseModel;
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
    m_expenseModel->load();
    ui->expenseTbl->setModel(m_expenseModel);
    m_formatter->format(ui->expenseTbl);
}

void App::loadForms() {

}

void App::on_addExpenseBtn_clicked() {
    m_addExpenseForm->show();
}

