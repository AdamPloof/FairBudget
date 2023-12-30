#ifndef PAYMENT_TABLE_H
#define PAYMENT_TABLE_H

#include <QWidget>

namespace Ui {
class PaymentTable;
}

class PaymentTable : public QWidget
{
    Q_OBJECT

public:
    explicit PaymentTable(QWidget *parent = nullptr);
    ~PaymentTable();

private:
    Ui::PaymentTable *ui;
};

#endif // PAYMENT_TABLE_H
