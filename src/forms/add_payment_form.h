#ifndef ADD_PAYMENT_FORM_H
#define ADD_PAYMENT_FORM_H

#include <QWidget>
#include <memory>

namespace Ui {
    class AddPaymentForm;
}

class EntityInterface;
class Person;
class Expense;
class IncomePeriod;

class AddPaymentForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddPaymentForm(QWidget *parent = nullptr);
    ~AddPaymentForm();

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
    std::shared_ptr<Person> fetchPerson(int id);
    std::shared_ptr<Expense> fetchExpense(int id);
    std::shared_ptr<IncomePeriod> fetchIncomePeriod(int id);

    Ui::AddPaymentForm *ui;
    double m_selectedExpenseAmt;
};

#endif // ADD_PAYMENT_FORM_H
