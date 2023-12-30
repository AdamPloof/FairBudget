#include "payment_table.h"
#include "ui_payment_table.h"

PaymentTable::PaymentTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PaymentTable)
{
    ui->setupUi(this);
}

PaymentTable::~PaymentTable()
{
    delete ui;
}
