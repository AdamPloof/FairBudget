#ifndef REPORT_BUILDER_H
#define REPORT_BUILDER_H

#include <QString>
#include <QList>
#include <QHash>
#include <memory>
#include "budget_calculator.h"

class EntityManager;
class EntityInterface;
class Person;
class Expense;
class Payment;
class PersonalBudget;

/**
 * ReportBuilder is responsible calculating totals, the amount owed
 * for each person and for generating the markdown used in
 * the owedReport QTextEdit widget.
 */
class ReportBuilder {
public:
    ReportBuilder(std::shared_ptr<EntityManager> em, BudgetCalculator calculator);
    QString build();
    QHash<int, PersonalBudget> calcBudgets(
        QList<std::shared_ptr<Person>> persons,
        QList<std::shared_ptr<Expense>> expenses,
        QList<std::shared_ptr<Payment>> payments
    );
    double calcHouseholdExpenses(QList<std::shared_ptr<Expense>> expenses) const;
    double calcHouseholdIncome(QList<std::shared_ptr<Person>> persons) const;
    double calcPaid(std::shared_ptr<Person> person) const;
    double calcOwed(std::shared_ptr<Person> person) const;
    double calcMonthlyIncome(std::shared_ptr<Person> person) const;

private:
    QString totalSection(double householdExpenses, double householdIncome) const;
    QString expenseSection(QHash<int, PersonalBudget> &budgets) const;
    QString paymentSection(QHash<int, PersonalBudget> &budgets) const;
    QString owedSection(QHash<int, PersonalBudget> &budgets) const;
    QString sectionSeparator() const;
    double calcUnpaidExpenses(QHash<int, PersonalBudget> &budgets) const;

    std::shared_ptr<EntityManager> m_entityManager;
    BudgetCalculator m_calculator;
};

#endif // REPORT_BUILDER_H
