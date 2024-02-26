#include "add_expense_form.h"
#include "ui_add_expense_form.h"
#include <QDebug>

AddExpenseForm::AddExpenseForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddExpenseForm)
{
    ui->setupUi(this);
}

AddExpenseForm::~AddExpenseForm()
{
    delete ui;
}

void AddExpenseForm::on_AddExpenseForm_accepted() {
    qDebug() << ui->descriptionInput->text();
    qDebug() << ui->amountInput->value();
}
