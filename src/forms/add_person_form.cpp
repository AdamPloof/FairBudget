#include "add_person_form.h"
#include "ui_add_person_form.h"
#include "../entities/expense.h"

AddPersonForm::AddPersonForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddPersonForm)
{
    ui->setupUi(this);
}

AddPersonForm::~AddPersonForm()
{
    delete ui;
}

void AddPersonForm::on_cancelBtn_clicked() {
    this->close();
}

void AddPersonForm::on_addBtn_clicked() {
    if (!isValid()) {
        this->close();
        return;
    }

    // TODO: use Person entity
    std::shared_ptr<Expense> expense = std::make_shared<Expense>(Expense());

    // emit submitPerson(expense);
    this->close();
}

bool AddPersonForm::isValid() {
    bool isValid = true;
    if (ui->nameInput->text().length() == 0) {
        isValid = false;
    }

    if (ui->incomeInput->value() <= 0) {
        isValid = false;
    }

    if (ui->periodSelect->currentData().isNull()) {
        isValid = false;
    }

    return isValid;
}
