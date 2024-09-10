#include <gtest/gtest.h>
#include <QList>
#include <QString>
#include <memory>
#include <cmath>
#include <iostream>

#include "../../src/entities/personal_budget.h"
#include "../../src/entities/person.h"
#include "../../src/entities/income_period.h"
#include "../../src/services/budget_calculator.h"

class BudgetManager {
public:
    static constexpr double HOUSEHOLD_EXPENSE = 3750.00;
    static constexpr double HOUSEHOLD_INCOME = 11850.00;

    BudgetManager() {
        std::shared_ptr<IncomePeriod> m_monthlyIncomePeriod = std::make_shared<IncomePeriod>(IncomePeriod());
        m_monthlyIncomePeriod->setData("id", 1);
        m_monthlyIncomePeriod->setData("period", "monthly");
        m_monthlyIncomePeriod->setData("label", "Monthly");
    }

    QList<PersonalBudget> buildBudgets() {
        QList<PersonalBudget> budgets = {
            {
                makePerson("Peter", 2500.00),
                2500.00,
                500,
                BudgetManager::HOUSEHOLD_EXPENSE,
                BudgetManager::HOUSEHOLD_INCOME
            },
            {
                makePerson("Paul", 4250.00),
                4250.00,
                2000,
                BudgetManager::HOUSEHOLD_EXPENSE,
                BudgetManager::HOUSEHOLD_INCOME
            },
            {
                makePerson("Mary", 3000.00),
                3000.00,
                250.0,
                BudgetManager::HOUSEHOLD_EXPENSE,
                BudgetManager::HOUSEHOLD_INCOME
            },
            {
                makePerson("Tyrone", 2100.00),
                2100.00,
                1000.00,
                BudgetManager::HOUSEHOLD_EXPENSE,
                BudgetManager::HOUSEHOLD_INCOME
            }
        };

        return budgets;
    }

private:
    std::shared_ptr<Person> makePerson(QString name, double income) {
        std::shared_ptr<Person> person = std::make_shared<Person>(Person());
        person->setData("id", m_personNextId);
        person->setData("name", name);
        person->setData("income", 2500.00);
        person->setIncomePeriod(m_monthlyIncomePeriod);
        m_personNextId++;

        return person;
    }

    int m_personNextId = 1;
    std::shared_ptr<IncomePeriod> m_monthlyIncomePeriod;
};


TEST(BudgetTest, ExpensesNotFullyPaid) {
    BudgetCalculator calculator;
    BudgetManager manager;
    QList<PersonalBudget> budgets = manager.buildBudgets();
    for (auto &budget : budgets) {
        budget.householdExpenses += 50.0;
    }

    QList<Debt> debts = calculator.calculateDebts(budgets);
    EXPECT_TRUE(debts.empty());
}

TEST(BudgetTest, ExpensesFullyPaid) {
    BudgetCalculator calculator;
    BudgetManager manager;
    QList<PersonalBudget> budgets = manager.buildBudgets();
    QList<Debt> debts = calculator.calculateDebts(budgets);
    EXPECT_TRUE(!debts.empty());
}

TEST(BudgetTest, DebitsAndCreditsBalance) {
    BudgetCalculator calculator;
    BudgetManager manager;
    QList<PersonalBudget> budgets = manager.buildBudgets();
    QList<Debt> debts = calculator.calculateDebts(budgets);
    double debits = 0.0;
    for (auto &debt : debts) {
        debits += debt.amount;

        std::cout << debt.debtor->getData("name").toString().toStdString() << " owes ";
        std::cout << debt.creditor->getData("name").toString().toStdString() << " " << debt.amount << std::endl;
    }

    double credits = 0.0;
    for (auto &budget : budgets) {
        credits += budget.outstanding() < 0 ? (budget.outstanding()) : 0.0;
    }

    double sum = std::round((debits + credits) * 100) / 100;

    EXPECT_EQ(sum, 0.0);
}
