#ifndef BUDGET_CALCULATOR_H
#define BUDGET_CALCULATOR_H

#include <QList>
#include <memory>
#include "../entities/person.h"
#include "../entities/personal_budget.h"

struct Debt {
    std::shared_ptr<Person> debtor;
    std::shared_ptr<Person> creditor;
    double amount;
};

struct Debtor : public PersonalBudget {
    QList<std::shared_ptr<Person>> owes;
    double paid_back = 0.0;
};

struct Creditor : public PersonalBudget {
    double reimbursed = 0.0;
};

class BudgetCalculator {
public:
    /**
     * Calculate debts owed between household members. If not all household
     * expenses have been paid, returns an empty QList.
     */
    QList<Debt> calculateDebts(const QList<PersonalBudget> &budgets) const;

    /**
     * Returns true if all household expenses in the budgets have been paid
     */
    bool expensesArePaid(const QList<PersonalBudget> &budgets) const;
private:
    Debtor makeDebtor(const PersonalBudget &budget) const;
    Creditor makeCreditor(const PersonalBudget &budget) const;
};

#endif // BUDGET_CALCULATOR_H
