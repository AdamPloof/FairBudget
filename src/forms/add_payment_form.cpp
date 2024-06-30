#include <QSqlQuery>

#include "add_payment_form.h"
#include "ui_add_payment_form.h"

AddPaymentForm::AddPaymentForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddPaymentForm)
{
    ui->setupUi(this);
}

AddPaymentForm::~AddPaymentForm()
{
    delete ui;
}

bool AddPaymentForm::isValid() {
    // TODO: check that amount not greater than expense amount
    return true;
}

void AddPaymentForm::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    setPersonOptions();
    setExpenseOptions();
}

void AddPaymentForm::on_cancelBtn_clicked() {
    this->close();
}

void AddPaymentForm::on_addBtn_clicked() {
    if (!isValid()) {
        this->close();
    }

    // TODO: submit the payment
    this->close();
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

    // TODO: on select expense option, change maximum value of amount input
}
