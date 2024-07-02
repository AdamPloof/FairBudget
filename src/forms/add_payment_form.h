#ifndef ADD_PAYMENT_FORM_H
#define ADD_PAYMENT_FORM_H

#include <QWidget>
#include <memory>

namespace Ui {
    class AddPaymentForm;
}

class EntityInterface;

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
    void setAmountMax(int index);

private:
    void setPersonOptions();
    void setExpenseOptions();

    Ui::AddPaymentForm *ui;
    double m_maxAmount;
};

#endif // ADD_PAYMENT_FORM_H
