#include <QSqlQuery>
#include <QSqlError>
#include <QComboBox>

#include "add_payment_form.h"
#include "ui_add_payment_form.h"

AddPaymentForm::AddPaymentForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddPaymentForm)
{
    ui->setupUi(this);

    QObject::connect(
        ui->expenseSelect,
        &QComboBox::currentIndexChanged,
        this,
        &AddPaymentForm::setAmountMax
    );
}

AddPaymentForm::~AddPaymentForm()
{
    delete ui;
}

bool AddPaymentForm::isValid() {
    return ui->amountInput->value() <= m_maxAmount;
}

void AddPaymentForm::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    setPersonOptions();
    setExpenseOptions();
    setAmountMax(ui->expenseSelect->currentIndex());
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

void AddPaymentForm::setAmountMax(int index) {
    QSqlQuery q;
    q.prepare("SELECT amount FROM expense WHERE id = :id");

    const int expId = ui->expenseSelect->itemData(index).toInt();
    q.bindValue(":id", expId);
    qDebug() << "Setting max payment amount for expense: " << expId;

    if (!q.exec()) {
        qDebug() << "Query execution error: " << q.lastError().text();
    } else {
        if (q.first()) {
            m_maxAmount = q.value(0).toDouble();
            qDebug() << "Max payment amount: " << m_maxAmount;
        } else {
            qDebug() << "No record found for id: " << expId;
        }
    }

    ui->amountInput->setMaximum(m_maxAmount);
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
