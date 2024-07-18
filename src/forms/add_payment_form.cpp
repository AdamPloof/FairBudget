#include <QSqlQuery>
#include <QSqlError>
#include <QComboBox>
#include <memory>
#include <sstream>

#include "add_payment_form.h"
#include "ui_add_payment_form.h"
#include "../entities/payment.h"
#include "../entities/person.h"
#include "../entities/expense.h"
#include "../entities/income_period.h"

AddPaymentForm::AddPaymentForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddPaymentForm)
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

    std::shared_ptr<Expense> exp = fetchExpense(ui->expenseSelect->currentData().toInt());
    payment->setExpense(exp);

    std::shared_ptr<Person> paidBy = fetchPerson(ui->personSelect->currentData().toInt());
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
    QSqlQuery q;
    q.prepare("SELECT amount FROM expense WHERE id = :id");

    const int expId = ui->expenseSelect->itemData(index).toInt();
    q.bindValue(":id", expId);
    qDebug() << "Setting selected expense amount: " << expId;

    if (!q.exec()) {
        qDebug() << "Query execution error: " << q.lastError().text();
    } else {
        if (q.first()) {
            m_selectedExpenseAmt = q.value(0).toDouble();
            qDebug() << "Selected expens amount: " << m_selectedExpenseAmt;
        } else {
            qDebug() << "No record found for id: " << expId;
        }
    }

    ui->amountInput->setValue(m_selectedExpenseAmt);
    ui->amountInput->setMaximum(m_selectedExpenseAmt);
}

void AddPaymentForm::setPersonOptions() {
    ui->personSelect->clear();
    QSqlQuery q = QSqlQuery("SELECT id, name FROM person");
    int idx = 0;
    while (q.next()) {
        ui->personSelect->insertItem(idx, q.value(1).toString(), q.value(0).toInt());
        idx++;
    }
}

void AddPaymentForm::setExpenseOptions() {
    ui->expenseSelect->clear();
    QSqlQuery q = QSqlQuery("SELECT id, description FROM expense");
    int idx = 0;
    while (q.next()) {
        ui->expenseSelect->insertItem(idx, q.value(1).toString(), q.value(0).toInt());
        idx++;
    }
}

std::shared_ptr<Person> AddPaymentForm::fetchPerson(int id) {
    QSqlQuery q;
    q.prepare("SELECT id, name, income, income_period FROM person WHERE id = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "Could not fetch person for id: " << id;
    }

    std::shared_ptr<Person> p = std::make_shared<Person>(Person());
    if (q.first()) {
        p->setData("id", id);
        p->setData("name", q.value(1));
        p->setData("income", q.value(2));
        p->setIncomePeriod(fetchIncomePeriod(q.value(3).toInt()));
    } else {
        std::stringstream err;
        err << "No person found for id: ";
        err << id;
        throw std::invalid_argument(err.str());
    }

    return p;
}

std::shared_ptr<Expense> AddPaymentForm::fetchExpense(int id) {
    QSqlQuery q;
    q.prepare("SELECT id, description, amount FROM expense WHERE id = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "Could not fetch expense for id: " << id;
    }

    std::shared_ptr<Expense> e = std::make_shared<Expense>(Expense());
    if (q.first()) {
        e->setData("id", id);
        e->setData("description", q.value(1));
        e->setData("amount", q.value(2));
    } else {
        std::stringstream err;
        err << "No expense found for id: ";
        err << id;
        throw std::invalid_argument(err.str());
    }

    return e;
}

std::shared_ptr<IncomePeriod> AddPaymentForm::fetchIncomePeriod(int id) {
    QSqlQuery q;
    q.prepare("SELECT id, period, label FROM income_period WHERE id = :id");
    q.bindValue(":id", id);

    if (!q.exec()) {
        qDebug() << "Could not fetch income period for id: " << id;
    }

    std::shared_ptr<IncomePeriod> ip = std::make_shared<IncomePeriod>(IncomePeriod());
    if (q.first()) {
        ip->setData("id", id);
        ip->setData("period", q.value(1));
        ip->setData("label", q.value(2));
    } else {
        std::stringstream err;
        err << "No income period found for id: ";
        err << id;
        throw std::invalid_argument(err.str());
    }

    return ip;
}
