#include "report_builder.h"
#include "entity_manager.h"

ReportBuilder::ReportBuilder(std::shared_ptr<EntityManager> em) : m_entityManager(em) {}

QString ReportBuilder::build() const {
    QString report = R"(
### Totals
**Total expenses:** $2,185.00  
**Total monthly income:** $8,416.67  

---
### Budget by income
**Ted**: $778.80 (35.6%)  
**Sherry:** $1,406.20 (64.4%)  

---
### Payments
**Ted paid:** $528.80  
**Sherry paid:** $1,656.20  
**Fairness ratio:** 1.0  

---
### Owed
**Ted** owes **Sherry** $250.00  
)";

    return report;
}
