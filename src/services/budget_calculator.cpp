#include <cmath>
#include <memory>

#include "budget_calculator.h"
#include "../entities/person.h"
#include "../entities/personal_budget.h"

QList<Debt> BudgetCalculator::calculateDebts(const QList<PersonalBudget> &budgets) const {
    QList<Debt> debts;
    if (!expensesArePaid(budgets)) {
        return debts;
    }

    QList<Debtor> debtors;
    QList<Creditor> creditors;
    for (auto budget : budgets) {
        const double outstanding = std::round(budget.outstanding() * 100 / 100);
        if (outstanding > 0.0) {
            debtors.append(makeDebtor(budget));
        } else if (outstanding < 0.0) {
            creditors.append(makeCreditor(budget));
        }
    }

    for (auto &debtor : debtors) {
        for (auto &creditor : creditors) {
            if (creditor.reimbursed >= creditor.outstanding() * -1) {
                // this creditor has already been paid back fully
                continue;
            }

            Debt debt;
            debt.debtor = debtor.person;
            debt.creditor = creditor.person;

            double debtorBalance = debtor.outstanding() - debtor.paid_back;
            double creditorBalance = (creditor.outstanding() * -1) - creditor.reimbursed;
            if (creditorBalance >= debtorBalance) {
                // in other words, if the creditor is owed more than the debtor owes,
                // the debtor can pay off what they owe in full to this creditor
                debt.amount = debtorBalance;
                debtor.paid_back = debtor.outstanding();
                creditor.reimbursed += debtorBalance;
            } else {
                // Debtor can only pay creditor equal to the amonut the creditor is still owed
                debt.amount = creditorBalance;
                debtor.paid_back += creditorBalance;
                creditor.reimbursed += creditorBalance;
            }

            debts.append(debt);
        }
    }

    return debts;
}

bool BudgetCalculator::expensesArePaid(const QList<PersonalBudget> &budgets) const {
    if (budgets.empty()) {
        return true;
    }

    double paid = 0.0;
    for (auto &budget : budgets) {
        paid += budget.paid;
    }

    paid = std::round(paid * 100) / 100;
    double householdExpenses = std::round(budgets.at(0).householdExpenses * 100) / 100;

    return paid == householdExpenses;
}

Debtor BudgetCalculator::makeDebtor(const PersonalBudget &budget) const {
    Debtor debtor;
    debtor.person = budget.person;
    debtor.monthlyIncome = budget.monthlyIncome;
    debtor.paid = budget.paid;
    debtor.householdExpenses = budget.householdExpenses;
    debtor.householdIncome = budget.householdIncome;
    debtor.fairnessRatio = budget.fairnessRatio;

    debtor.owes = QList<std::shared_ptr<Person>>();
    debtor.paid_back = 0.0;

    return debtor;
}

Creditor BudgetCalculator::makeCreditor(const PersonalBudget &budget) const {
    Creditor creditor;
    creditor.person = budget.person;
    creditor.monthlyIncome = budget.monthlyIncome;
    creditor.paid = budget.paid;
    creditor.householdExpenses = budget.householdExpenses;
    creditor.householdIncome = budget.householdIncome;
    creditor.fairnessRatio = budget.fairnessRatio;
    creditor.reimbursed = 0.0;

    return creditor;
}
