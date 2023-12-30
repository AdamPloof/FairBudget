#ifndef EXPENSE_TABLE_H
#define EXPENSE_TABLE_H

#include <QWidget>

namespace Ui {
class ExpenseTable;
}

class ExpenseTable : public QWidget
{
    Q_OBJECT

public:
    explicit ExpenseTable(QWidget *parent = nullptr);
    ~ExpenseTable();

private:
    Ui::ExpenseTable *ui;
};

#endif // EXPENSE_TABLE_H
