#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include "expense_table.h"
#include "person_table.h"
#include "payment_table.h"
#include "owed_table.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class App;
}
QT_END_NAMESPACE

class App : public QMainWindow
{
    Q_OBJECT

public:
    App(QWidget *parent = nullptr);
    ~App();

    void loadChildren();

private:
    Ui::App *ui;
    ExpenseTable* m_expenseTbl;
    PersonTable* m_personTbl;
    PaymentTable* m_paymentTbl;
    OwedTable* m_owedTbl;
};
#endif // APP_H
