#ifndef ADD_EXPENSE_FORM_H
#define ADD_EXPENSE_FORM_H

#include <QDialog>

namespace Ui {
class AddExpenseForm;
}

class AddExpenseForm : public QDialog
{
    Q_OBJECT

public:
    explicit AddExpenseForm(QWidget *parent = nullptr);
    ~AddExpenseForm();

private slots:
    void on_AddExpenseForm_accepted();

private:
    Ui::AddExpenseForm *ui;
};

#endif // ADD_EXPENSE_FORM_H
