#include <gtest/gtest.h>
#include <QList>
#include <QString>
#include <QDebug>
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
    }

    double credits = 0.0;
    for (auto &budget : budgets) {
        credits += budget.outstanding() < 0 ? (budget.outstanding()) : 0.0;
    }

    double sum = std::round((debits + credits) * 100) / 100;

    EXPECT_EQ(sum, 0.0);
}

TEST(BudgetTest, DebtCountIsCorrect) {
    BudgetCalculator calculator;
    BudgetManager manager;
    QList<PersonalBudget> budgets = manager.buildBudgets();
    QList<Debt> debts = calculator.calculateDebts(budgets);
    EXPECT_EQ(debts.size(), 3);
}

TEST(BudgetTest, DebtsAndPaidEqualExpenses) {
    BudgetCalculator calculator;
    BudgetManager manager;
    QList<PersonalBudget> budgets = manager.buildBudgets();
    QList<Debt> debts = calculator.calculateDebts(budgets);
    double totalDebt = 0.0;
    for (auto &debt : debts) {
        totalDebt += debt.amount;
    }

    double totalPaidLessDebt = 0.0;
    for (auto &budget : budgets) {
        if (budget.outstanding() <= 0) {
            totalPaidLessDebt += budget.outstanding() + budget.paid;
        } else {
            totalPaidLessDebt += budget.paid;
        }
    }

    double totalExpenses = budgets.at(0).householdExpenses;

    EXPECT_EQ(totalDebt + totalPaidLessDebt, totalExpenses);
}

TEST(BudgetTest, DebtsEqualOwedMinusPaid) {
    BudgetCalculator calculator;
    BudgetManager manager;
    QList<PersonalBudget> budgets = manager.buildBudgets();
    QList<Debt> debts = calculator.calculateDebts(budgets);
    double peterDebt = 0.0;
    double paulDebt = 0.0;
    double maryDebt = 0.0;
    double tyroneDebt = 0.0;
    for (auto &debt : debts) {
        QString debtorName = debt.debtor->getData("name").toString();

        if (debtorName == "Peter") {
            peterDebt += debt.amount;
        } else if (debtorName == "Paul") {
            paulDebt += debt.amount;
        } else if (debtorName == "Mary") {
            maryDebt += debt.amount;
        } else if (debtorName == "Tyrone") {
            tyroneDebt += debt.amount;
        }
    }

    EXPECT_EQ(std::round(peterDebt * 100) / 100, 291.14);
    EXPECT_EQ(std::round(paulDebt * 100) / 100, 0.0);
    EXPECT_EQ(std::round(maryDebt * 100) / 100, 699.37);
    EXPECT_EQ(std::round(tyroneDebt * 100) / 100, 0.0);

    double paulOwes = -655.06;
    double tyroneOwes = -335.44;
}

TEST(BudgetTest, OwedEqualDebts) {
    BudgetCalculator calculator;
    BudgetManager manager;
    QList<PersonalBudget> budgets = manager.buildBudgets();
    QList<Debt> debts = calculator.calculateDebts(budgets);
    double peterCredit = 0.0;
    double paulCredit = 0.0;
    double maryCredit = 0.0;
    double tyroneCredit = 0.0;
    for (auto &debt : debts) {
        QString creditorName = debt.creditor->getData("name").toString();

        if (creditorName == "Peter") {
            peterCredit += debt.amount;
        } else if (creditorName == "Paul") {
            paulCredit += debt.amount;
        } else if (creditorName == "Mary") {
            maryCredit += debt.amount;
        } else if (creditorName == "Tyrone") {
            tyroneCredit += debt.amount;
        }
    }

    EXPECT_EQ(std::round(peterCredit * 100) / 100, 0.0);
    EXPECT_EQ(std::round(paulCredit * 100) / 100, 655.06);
    EXPECT_EQ(std::round(maryCredit * 100) / 100, 0.0);
    EXPECT_EQ(std::round(tyroneCredit * 100) / 100, 335.44);
}
