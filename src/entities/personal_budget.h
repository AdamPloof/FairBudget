#ifndef PERSONAL_BUDGET_H
#define PERSONAL_BUDGET_H

#include <memory>
#include "person.h"

struct PersonalBudget {
    std::shared_ptr<Person> person;
    double monthlyIncome = 0.0;
    double paid = 0.0;
    double householdExpenses = 0.0;
    double householdIncome = 0.0;

    double incomeRatio() const;
    double owes() const;
    double outstanding() const;
};

#endif // PERSONAL_BUDGET_H
