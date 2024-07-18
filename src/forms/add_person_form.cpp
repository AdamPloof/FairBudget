#include <QList>
#include <QSqlQuery>
#include <QDebug>
#include <sstream>

#include "add_person_form.h"
#include "ui_add_person_form.h"
#include "../entities/person.h"
#include "../entities/income_period.h"

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

    std::shared_ptr<IncomePeriod> period = fetchIncomePeriod(ui->periodSelect->currentData().toInt());
    person->setIncomePeriod(period);

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

    // TODO: validate selected period option

    return isValid;
}

void AddPersonForm::setPeriodOptions() {
    ui->periodSelect->clear();
    QSqlQuery q = QSqlQuery("SELECT id, label FROM income_period");
    int idx = 0;
    while (q.next()) {
        ui->periodSelect->insertItem(idx, q.value(1).toString(), q.value(0).toInt());
        idx++;
    }
}

std::shared_ptr<IncomePeriod> AddPersonForm::fetchIncomePeriod(int id) {
    QSqlQuery q;
    q.prepare("SELECT id, period, label FROM income_period WHERE id = :id");
    q.bindValue(":id", id);
    if (!q.exec()) {
        qDebug() << "Could not fetch income period: " << id;
    }

    std::shared_ptr<IncomePeriod> ip = std::make_shared<IncomePeriod>(IncomePeriod());
    if (q.first()) {
        ip->setData("id", id);
        ip->setData("period", q.value(1).toString());
        ip->setData("label", q.value(2).toString());
    } else {
        std::stringstream err;
        err << "Could not fetch income period for ID: ";
        err << id;
        throw std::invalid_argument(err.str());
    }

    return ip;
}
