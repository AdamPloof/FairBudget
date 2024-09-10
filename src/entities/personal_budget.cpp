#include "personal_budget.h"

double PersonalBudget::incomeRatio() const {
    return monthlyIncome / householdIncome;
}

double PersonalBudget::owes() const {
    return householdExpenses * incomeRatio();
}

double PersonalBudget::outstanding() const {
    return owes() - paid;
}
