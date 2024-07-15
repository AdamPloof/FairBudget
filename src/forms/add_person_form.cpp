#include <QList>
#include "add_person_form.h"
#include "ui_add_person_form.h"
#include "../entities/person.h"

AddPersonForm::AddPersonForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddPersonForm)
{
    ui->setupUi(this);
    setPeriodOptions();
}

AddPersonForm::~AddPersonForm()
{
    delete ui;
}

QHash<QString, QVariant> AddPersonForm::periodOptions = {
    {"Weekly", "weekly"},
    {"Bi-weekly", "bi_weekly"},
    {"Monthly", "monthly"},
    {"Annually", "annually"}
};

void AddPersonForm::on_cancelBtn_clicked() {
    this->close();
}

void AddPersonForm::on_addBtn_clicked() {
    if (!isValid()) {
        this->close();
        return;
    }

    std::shared_ptr<Person> person = std::make_shared<Person>(Person());
    person->setData("name", ui->nameInput->text());
    person->setData("income", ui->incomeInput->value());
    person->setData("income_period", ui->periodSelect->currentData());

    emit submitPerson(person);
    this->close();
}

void AddPersonForm::setName(const QString &name) {
    ui->nameInput->setText(name);
}

void AddPersonForm::setIncome(double income) {
    ui->incomeInput->setValue(income);
}

void AddPersonForm::setIncomePeriod(const QString &period) {
    int periodIdx = ui->periodSelect->findText(period);
    if (periodIdx == -1) {
        return;
    }

    ui->periodSelect->setCurrentIndex(periodIdx);
}

bool AddPersonForm::isValid() {
    bool isValid = true;
    if (ui->nameInput->text().length() == 0) {
        isValid = false;
    }

    if (ui->incomeInput->value() <= 0) {
        isValid = false;
    }

    QList<QVariant> opts = AddPersonForm::periodOptions.values();
    QString period = ui->periodSelect->currentData().toString();
    bool validPeriod = false;
    for (auto opt : opts) {
        if (opt.toString() == period) {
            validPeriod = true;
            break;
        }
    }

    if (!validPeriod) {
        isValid = false;
    }

    return isValid;
}

void AddPersonForm::setPeriodOptions() {
    QHash<QString, QVariant>::const_iterator i = AddPersonForm::periodOptions.constBegin();
    while (i != AddPersonForm::periodOptions.constEnd()) {
        int optIdx = ui->periodSelect->findText(i.key());
        if (optIdx != -1) {
            ui->periodSelect->setItemData(optIdx, i.value());
        }

        i++;
    }
}
