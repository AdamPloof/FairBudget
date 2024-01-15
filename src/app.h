#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <memory>
#include "services/entity_manager.h"
#include "models/expense.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class App;
}
QT_END_NAMESPACE


class App : public QMainWindow
{
    Q_OBJECT

public:
    App(std::shared_ptr<EntityManager> em, QWidget *parent = nullptr);
    ~App();

    void run();
    void loadDb();

private:
    Ui::App *ui;
    Expense* m_expense;
    std::shared_ptr<EntityManager> m_entityManager;
};
#endif // APP_H
