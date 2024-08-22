#ifndef REPORT_BUILDER_H
#define REPORT_BUILDER_H

#include <QString>
#include <memory>

class EntityManager;

/**
 * ReportBuilder is responsible for generating the markdown
 * used in the owedReport QTextEdit widget. Basically, the results
 * of the owed calculator plus some other basic stats about the expenses
 * and incomes. 
 */
class ReportBuilder {
public:
    ReportBuilder(std::shared_ptr<EntityManager> em);
    QString build() const;

private:
    std::shared_ptr<EntityManager> m_entityManager;
};

#endif // REPORT_BUILDER_H
