#include <QList>
#include <QDebug>
#include <sstream>

#include "add_person_form.h"
#include "ui_add_person_form.h"
#include "../services/entity_manager.h"
#include "../entities/entity_interface.h"
#include "../entities/person.h"
#include "../entities/income_period.h"

AddPersonForm::AddPersonForm(std::shared_ptr<EntityManager> em, QWidget *parent)
    : QWidget(parent), 
    m_entityManager(em),
    ui(new Ui::AddPersonForm)
{
    ui->setupUi(this);
}

AddPersonForm::~AddPersonForm()
{
    delete ui;
}

void AddPersonForm::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);
    setPeriodOptions();
}

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

    std::shared_ptr<IncomePeriod> period = m_entityManager->find<IncomePeriod>(
        ui->periodSelect->currentData().toInt()
    );
    person->setIncomePeriod(period);

    emit submitPerson(person);
    this->close();
    clearInputs();
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

    return isValid;
}

void AddPersonForm::setPeriodOptions() {
    ui->periodSelect->clear();
    QList<std::shared_ptr<EntityInterface>> periods = m_entityManager->findAll(EntityType::INCOME_PERIOD, true);
    int idx = 0;
    for (const auto &period : periods) {
        ui->periodSelect->insertItem(idx, period->getData("label").toString(), period->getId());
        idx++;
    }
}

void AddPersonForm::clearInputs() {
    setName("");
    setIncome(0.0);
}
