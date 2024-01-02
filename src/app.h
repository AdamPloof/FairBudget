#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include "services/entity_manager.h"

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

    void loadDb();

private:
    Ui::App *ui;
    EntityManager m_entityManager;
};
#endif // APP_H
