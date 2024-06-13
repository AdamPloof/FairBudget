#include <QString>
#include <QDebug>
#include <vector>
#include <QTableView>
#include <QItemSelection>

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
    connectForms();
    connectButtons();
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

void App::connectForms() {
    QObject::connect(
        m_addExpenseForm,
        &AddExpenseForm::submitExpense,
        m_expenseModel,
        &ExpenseModel::addExpense
    );
}

void App::connectButtons() {
    QObject::connect(
        ui->expenseTbl->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this,
        &App::on_expenseSelectionChanged
    );
}

void App::on_addExpenseBtn_clicked() {
    m_addExpenseForm->show();
}

void App::on_removeExpenseBtn_clicked() {
    QItemSelection selection = ui->expenseTbl->selectionModel()->selection();
    if (selection.indexes().isEmpty()) {
        return;
    }

    int row = selection.indexes().first().row();
    std::shared_ptr<EntityInterface> expense = m_expenseModel->getRow(row);
    if (expense == nullptr) {
        return;
    }

    m_expenseModel->removeExpense(expense);
}

void App::on_expenseSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    if (!selected.indexes().isEmpty()) {
        ui->removeExpenseBtn->setEnabled(true);
        QModelIndex index = selected.indexes().first();
        qDebug() << "Row" << index.row() << "selected";
    } else {
        ui->removeExpenseBtn->setEnabled(false);
        qDebug() << "Selection empty";
    }
}
