#include <QList>
#include <QSqlError>
#include <QComboBox>
#include <memory>
#include <sstream>

#include "add_payment_form.h"
#include "ui_add_payment_form.h"
#include "../services/entity_manager.h"
#include "../entities/entity_interface.h"
#include "../entities/payment.h"
#include "../entities/person.h"
#include "../entities/expense.h"
#include "../entities/income_period.h"

AddPaymentForm::AddPaymentForm(std::shared_ptr<EntityManager> em, QWidget *parent)
    : QWidget(parent),
    m_entityManager(em),
    ui(new Ui::AddPaymentForm)
{
    ui->setupUi(this);

    QObject::connect(
        ui->expenseSelect,
        &QComboBox::currentIndexChanged,
        this,
        &AddPaymentForm::on_selectExpense
    );
}

AddPaymentForm::~AddPaymentForm()
{
    delete ui;
}

bool AddPaymentForm::isValid() {
    bool isValid = true;
    if (!ui->personSelect->currentData().isValid()) {
        isValid = false;
    }

    if (!ui->expenseSelect->currentData().isValid()) {
        isValid = false;
    }

    // TODO: Shouldn't this be >=? -- add test
    if (ui->amountInput->value() <= m_selectedExpenseAmt) {
        isValid = false;
    }

    return isValid;
}

void AddPaymentForm::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    setPersonOptions();
    setExpenseOptions();
    on_selectExpense(ui->expenseSelect->currentIndex());
}

void AddPaymentForm::on_cancelBtn_clicked() {
    this->close();
}

void AddPaymentForm::on_addBtn_clicked() {
    if (!isValid()) {
        this->close();
    }

    std::shared_ptr<Payment> payment = std::make_shared<Payment>(Payment());
    payment->setData("amount", ui->amountInput->value());

    std::shared_ptr<Expense> exp = m_entityManager->find<Expense>(ui->expenseSelect->currentData().toInt());
    payment->setExpense(exp);

    std::shared_ptr<Person> paidBy = m_entityManager->find<Person>(ui->personSelect->currentData().toInt());
    payment->setPaidBy(paidBy);

    emit submitPayment(payment);
    this->close();
}

/**
 * Set the the amount input to the expense amount
 * Set the maximum amount to the selected expense amount
 * 
 * TODO: limit max amount to total of expense amonut minus other payments agains
 *       that expense.
 */
void AddPaymentForm::on_selectExpense(int index) {
    const int expId = ui->expenseSelect->itemData(index).toInt();
    std::shared_ptr<EntityInterface> expense = m_entityManager->find<Expense>(expId);

    qDebug() << "Setting selected expense amount: " << expId;
    if (expense != nullptr) {
        m_selectedExpenseAmt = expense->getData("amount").toDouble();
        qDebug() << "Selected expense amount: " << m_selectedExpenseAmt;
    } else {
        qDebug() << "No expense found for id: " << expId;
    }

    ui->amountInput->setValue(m_selectedExpenseAmt);
    ui->amountInput->setMaximum(m_selectedExpenseAmt);
}

void AddPaymentForm::setPersonOptions() {
    ui->personSelect->clear();
    QList<std::shared_ptr<EntityInterface>> persons = m_entityManager->findAll(EntityType::PERSON);

    int idx = 0;
    for (const auto &person : persons) {
        ui->personSelect->insertItem(idx, person->getData("name").toString(), person->getId());
        idx++;
    }
}

void AddPaymentForm::setExpenseOptions() {
    ui->expenseSelect->clear();
    QList<std::shared_ptr<EntityInterface>> expenses = m_entityManager->findAll(EntityType::EXPENSE);

    int idx = 0;
    for (const auto &expense : expenses) {
        ui->expenseSelect->insertItem(idx, expense->getData("description").toString(), expense->getId());
        idx++;
    }
}
