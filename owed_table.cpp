#include "owed_table.h"
#include "ui_owed_table.h"

OwedTable::OwedTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OwedTable)
{
    ui->setupUi(this);
}

OwedTable::~OwedTable()
{
    delete ui;
}
