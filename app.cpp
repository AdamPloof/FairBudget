#include "app.h"
#include "./ui_app.h"

App::App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);
    loadChildren();
}

App::~App()
{
    delete ui;
}

void App::loadChildren() {
    m_expenseTbl = new ExpenseTable(this);
    m_personTbl = new PersonTable(this);
    m_paymentTbl = new PaymentTable(this);
    m_owedTbl = new OwedTable(this);

    ui->mainLayout->addWidget(m_expenseTbl);
    ui->mainLayout->addWidget(m_personTbl);
    ui->mainLayout->addWidget(m_paymentTbl);
    ui->mainLayout->addWidget(m_owedTbl);
}
