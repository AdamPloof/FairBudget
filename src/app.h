#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include <memory>
#include "services/entity_manager.h"
#include "services/table_formatter.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class App;
}
QT_END_NAMESPACE

class QTableView;

class App : public QMainWindow
{
    Q_OBJECT

public:
    App(std::shared_ptr<EntityManager> em, QWidget *parent = nullptr);
    ~App();

    void run();
    void setFormatter(TableFormatter* formatter);
    void loadDb();

private slots:
    void on_addExpenseBtn_clicked();

private:
    void loadTables();
    void loadForms();
    void formatTable(QTableView* tbl);

    Ui::App *ui;
    TableFormatter* m_formatter;

    std::shared_ptr<EntityManager> m_entityManager;
};

#endif // APP_H
