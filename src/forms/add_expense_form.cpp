#include <QDebug>
#include <memory>

#include "ui_add_expense_form.h"
#include "add_expense_form.h"
#include "../entities/expense.h"

AddExpenseForm::AddExpenseForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddExpenseForm)
{
    ui->setupUi(this);
}

AddExpenseForm::~AddExpenseForm()
{
    delete ui;
}

void AddExpenseForm::on_cancelBtn_clicked() {
    this->close();
}

void AddExpenseForm::on_addBtn_clicked() {
    if (!isValid()) {
        this->close();
        return;
    }

    std::shared_ptr<Expense> expense = std::make_shared<Expense>(Expense());
    expense->setData("description", ui->descriptionInput->text());
    expense->setData("amount", ui->amountInput->value());
    emit submitExpense(expense);
    this->close();
}

void AddExpenseForm::setDescription(const QString &description) {
    ui->descriptionInput->setText(description);
}

void AddExpenseForm::setAmount(double amount) {
    ui->amountInput->setValue(amount);
}

bool AddExpenseForm::isValid() {
    bool isValid = true;
    if (ui->descriptionInput->text().length() == 0) {
        isValid = false;
    }

    if (ui->amountInput->value() <= 0) {
        isValid = false;
    }

    return isValid;
}
