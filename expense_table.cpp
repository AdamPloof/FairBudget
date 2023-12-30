#include "expense_table.h"
#include "ui_expense_table.h"

ExpenseTable::ExpenseTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ExpenseTable)
{
    ui->setupUi(this);
}

ExpenseTable::~ExpenseTable()
{
    delete ui;
}
