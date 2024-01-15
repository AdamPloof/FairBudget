#include "app.h"

#include <QApplication>
#include <memory>
#include "services/entity_manager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<EntityManager> em = std::make_shared<EntityManager>();
    App w = App(em);
    w.run();
    w.show();
    return a.exec();
}
