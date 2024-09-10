#include "personal_budget.h"

double PersonalBudget::owes() const {
    return householdExpenses * fairnessRatio;
}

double PersonalBudget::outstanding() const {
    return owes() - paid;
}
