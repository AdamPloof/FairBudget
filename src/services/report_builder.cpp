#include <sstream>
#include <string>

#include "report_builder.h"
#include "entity_manager.h"
#include "../entities/entity_interface.h"
#include "../entities/expense.h"
#include "../entities/person.h"
#include "../entities/payment.h"
#include "../entities/income_period.h"
#include "../entities/personal_budget.h"

ReportBuilder::ReportBuilder(std::shared_ptr<EntityManager> em) : m_entityManager(em) {}

// TODO: how to handle when not all bills have been paid?
QString ReportBuilder::build() {
    QList<std::shared_ptr<Expense>> expenses = m_entityManager->findAll<Expense>();
    QList<std::shared_ptr<Person>> persons = m_entityManager->findAll<Person>();
    QList<std::shared_ptr<Payment>> payments = m_entityManager->findAll<Payment>();
    QHash<int, PersonalBudget> budgets = calcBudgets(persons, expenses, payments);

    double householdExpenses;
    double householdIncome;
    if (budgets.empty()) {
        // If no persons calc expense total separately, otherwise get from first budget
        householdExpenses = calcHouseholdExpenses(expenses);
        householdIncome = 0.0;
    } else {
        householdExpenses = budgets.begin()->householdExpenses;
        householdIncome = budgets.begin()->householdIncome;
    }

    QString report;
    report.append(totalSection(householdExpenses, householdIncome));
    report.append(sectionSeparator());
    report.append(expenseSection(budgets));
    report.append(sectionSeparator());
    report.append(paymentSection(budgets));
    report.append(sectionSeparator());
    report.append(owedSection(budgets));

    return report;
}

// Assumes that total expenses and total income have been calculated
QHash<int, PersonalBudget> ReportBuilder::calcBudgets(
    QList<std::shared_ptr<Person>> persons,
    QList<std::shared_ptr<Expense>> expenses,
    QList<std::shared_ptr<Payment>> payments
) {
    QHash<int, PersonalBudget> budgets;
    if (persons.empty()) {
        return budgets;
    }

    double householdExpenses = calcHouseholdExpenses(expenses);
    double householdIncome = calcHouseholdIncome(persons);
    for (auto person : persons) {
        int id = person->getId();
        budgets.insert(id, PersonalBudget());
        budgets[id].person = person;
        budgets[id].householdExpenses = householdExpenses;
        budgets[id].householdIncome = householdIncome;
        budgets[id].monthlyIncome = calcMonthlyIncome(person);
        budgets[id].fairnessRatio = budgets[id].monthlyIncome / householdIncome;
    }

    for (auto payment : payments) {
        int personId = payment->getPaidBy()->getId();
        budgets[personId].paid += payment->getData("amount").toDouble();
    }

    return budgets;
}

double ReportBuilder::calcHouseholdExpenses(QList<std::shared_ptr<Expense>> expenses) const {
    double totalExpenses = 0.0;
    for (auto exp : expenses) {
        totalExpenses += exp->getData("amount").toDouble();
    }

    return totalExpenses;
}

double ReportBuilder::calcHouseholdIncome(QList<std::shared_ptr<Person>> persons) const {
    double totalIncome = 0.0;
    for (auto p : persons) {
        totalIncome += calcMonthlyIncome(p);
    }

    return totalIncome;
}

double ReportBuilder::calcPaid(std::shared_ptr<Person> person) const {
    return 0.0;
}

double ReportBuilder::calcOwed(std::shared_ptr<Person> person) const {
    return 0.0;
}

double ReportBuilder::calcMonthlyIncome(std::shared_ptr<Person> person) const {
    std::shared_ptr<IncomePeriod> ip = person->getIncomePeriod();
    QString period = ip->getData("period").toString();
    int multiplier;
    if (period == "weekly") {
        multiplier = 52;
    } else if (period == "bi_weekly") {
        multiplier = 26;
    } else if (period == "monthly") {
        multiplier = 12;
    } else if (period == "annually") {
        multiplier = 1;
    } else {
        std::string err = "Invalid income period provided: " + period.toStdString();
        throw std::runtime_error(err);
    }

    double income = person->getData("income").toDouble();

    return (income * multiplier) / 12;
}

QString ReportBuilder::totalSection(double householdExpenses, double householdIncome) const {
    std::stringstream section;
    section << "### Totals\n";
    section << "**Expenses:** $" << householdExpenses << "  \n";
    section << "**Monthly income:** $" << householdIncome << "  \n";

    QString qSection = section.str().c_str();

    return qSection;
}

QString ReportBuilder::expenseSection(QHash<int, PersonalBudget> budgets) const {
    std::stringstream section;
    section << "### Expenses\n";

    for (auto budget : budgets) {
        section << "**" << budget.person->getData("name").toString().toStdString() << " owes**: $";
        section << budget.owes() << " (" << budget.fairnessRatio << "%)  \n";
    }

    QString qSection = section.str().c_str();

    return qSection;
}

QString ReportBuilder::paymentSection(QHash<int, PersonalBudget> budgets) const {
    std::stringstream section;
    section << "### Payments\n";

    for (auto budget : budgets) {
        section << "**" << budget.person->getData("name").toString().toStdString() << " paid**: $";
        section << budget.paid << "  \n";
    }

    QString qSection = section.str().c_str();

    return qSection;
}

QString ReportBuilder::owedSection(QHash<int, PersonalBudget> budgets) const {
    QString section = R"(
### Owed
**Ted** owes **Sherry** $250.00  
)";

    return section;
}

QString ReportBuilder::sectionSeparator() const {
    return "\n\n---\n";
}
