#include <QString>
#include <QDebug>
#include <vector>
#include <QTableView>
#include <QItemSelection>

#include "app.h"
#include "./ui_app.h"
#include "models/expense_model.h"
#include "models/person_model.h"
#include "models/payment_model.h"

App::App(std::shared_ptr<EntityManager> em, QWidget *parent)
    : QMainWindow(parent),
    m_entityManager(em),
    ui(new Ui::App)
{
    ui->setupUi(this);

    m_addExpenseForm = new AddExpenseForm(this);
    m_addExpenseForm->setWindowFlag(Qt::Window);

    m_addPersonForm = new AddPersonForm(this);
    m_addPersonForm->setWindowFlag(Qt::Window);

    m_addPaymentForm = new AddPaymentForm(this);
    m_addPaymentForm->setWindowFlag(Qt::Window);

    m_expenseModel = new ExpenseModel(em);
    m_personModel = new PersonModel(em);
    m_paymentModel = new PaymentModel(em);
}

App::~App()
{
    delete ui;
    delete m_addExpenseForm;
    delete m_addPersonForm;
    delete m_addPaymentForm;
    delete m_expenseModel;
    delete m_personModel;
    delete m_paymentModel;
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
    // Expenses
    m_expenseModel->load();
    ui->expenseTbl->setModel(m_expenseModel);
    m_formatter->format(ui->expenseTbl);

    // Persons
    m_personModel->load();
    ui->personTbl->setModel(m_personModel);
    m_formatter->format(ui->personTbl);

    // Payments
    m_paymentModel->load();
    ui->paymentTbl->setModel(m_paymentModel);
    m_formatter->format(ui->paymentTbl);
}

void App::connectForms() {
    // Expense form
    QObject::connect(
        m_addExpenseForm,
        &AddExpenseForm::submitExpense,
        m_expenseModel,
        &ExpenseModel::addExpense
    );

    // Person form
    QObject::connect(
        m_addPersonForm,
        &AddPersonForm::submitPerson,
        m_personModel,
        &PersonModel::addPerson
    );

    // Payment form
    QObject::connect(
        m_addPaymentForm,
        &AddPaymentForm::submitPayment,
        m_paymentModel,
        &PaymentModel::addPayment
    );
}

void App::connectButtons() {
    // Expense table selection --> remove expense button
    QObject::connect(
        ui->expenseTbl->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this,
        &App::on_expenseSelectionChanged
    );

    // Person table selection --> remove person button
    QObject::connect(
        ui->personTbl->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this,
        &App::on_personSelectionChanged
    );

    // Payment table selection --> remove payment button
    QObject::connect(
        ui->paymentTbl->selectionModel(),
        &QItemSelectionModel::selectionChanged,
        this,
        &App::on_paymentSelectionChanged
    );
}

void App::on_addExpenseBtn_clicked() {
    m_addExpenseForm->show();
}

void App::on_addPersonBtn_clicked() {
    m_addPersonForm->show();
}

void App::on_addPaymentBtn_clicked() {
    m_addPaymentForm->show();
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

void App::on_removePersonBtn_clicked() {
    QItemSelection selection = ui->personTbl->selectionModel()->selection();
    if (selection.indexes().isEmpty()) {
        return;
    }

    int row = selection.indexes().first().row();
    std::shared_ptr<EntityInterface> person = m_personModel->getRow(row);
    if (person == nullptr) {
        return;
    }

    m_personModel->removePerson(person);
}

void App::on_removePaymentBtn_clicked() {
    QItemSelection selection = ui->paymentTbl->selectionModel()->selection();
    if (selection.indexes().isEmpty()) {
        return;
    }

    int row = selection.indexes().first().row();
    std::shared_ptr<EntityInterface> payment = m_paymentModel->getRow(row);
    if (payment == nullptr) {
        return;
    }

    m_paymentModel->removePayment(payment);
}

void App::on_expenseSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    if (!selected.indexes().isEmpty()) {
        ui->removeExpenseBtn->setEnabled(true);
        QModelIndex index = selected.indexes().first();
        qDebug() << "Expense row" << index.row() << "selected";
    } else {
        ui->removeExpenseBtn->setEnabled(false);
        qDebug() << "Expense selection empty";
    }
}

void App::on_personSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    if (!selected.indexes().isEmpty()) {
        ui->removePersonBtn->setEnabled(true);
        QModelIndex index = selected.indexes().first();
        qDebug() << "Person row" << index.row() << "selected";
    } else {
        ui->removePersonBtn->setEnabled(false);
        qDebug() << "Person selection empty";
    }
}

void App::on_paymentSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    if (!selected.indexes().isEmpty()) {
        ui->removePaymentBtn->setEnabled(true);
        QModelIndex index = selected.indexes().first();
        qDebug() << "Payment row" << index.row() << "selected";
    } else {
        ui->removePaymentBtn->setEnabled(false);
        qDebug() << "Payment selection empty";
    }
}
