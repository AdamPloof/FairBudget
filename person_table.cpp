#include "person_table.h"
#include "ui_person_table.h"

PersonTable::PersonTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PersonTable)
{
    ui->setupUi(this);
}

PersonTable::~PersonTable()
{
    delete ui;
}
