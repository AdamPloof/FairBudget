#ifndef ADD_PAYMENT_FORM_H
#define ADD_PAYMENT_FORM_H

#include <QWidget>
#include <memory>

namespace Ui {
    class AddPaymentForm;
}

class EntityManager;
class EntityInterface;
class Person;
class Expense;
class IncomePeriod;

class AddPaymentForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddPaymentForm(std::shared_ptr<EntityManager> em, QWidget *parent = nullptr);
    ~AddPaymentForm();

    void setAmount(double amount);
    bool isValid();

signals:
    void submitPayment(std::shared_ptr<EntityInterface> payment);

protected:
    void showEvent(QShowEvent *event) override;

private slots:
    void on_cancelBtn_clicked();
    void on_addBtn_clicked();
    void on_selectExpense(int index);

private:
    void setPersonOptions();
    void setExpenseOptions();
    void clearInputs();

    Ui::AddPaymentForm *ui;
    std::shared_ptr<EntityManager> m_entityManager;
    double m_selectedExpenseAmt;
};

#endif // ADD_PAYMENT_FORM_H
