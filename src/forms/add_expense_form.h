#ifndef ADD_EXPENSE_FORM_H
#define ADD_EXPENSE_FORM_H

#include <QWidget>
#include <memory>

namespace Ui {
    class AddExpenseForm;
}

class EntityInterface;

class AddExpenseForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddExpenseForm(QWidget *parent = nullptr);
    ~AddExpenseForm();

    void setDescription(const QString &description);
    void setAmount(double amount);
    bool isValid();

signals:
    void submitExpense(std::shared_ptr<EntityInterface> expense);

private slots:
    void on_cancelBtn_clicked();
    void on_addBtn_clicked();

private:
    void clearInputs();

    Ui::AddExpenseForm *ui;
};

#endif // ADD_EXPENSE_FORM_H
