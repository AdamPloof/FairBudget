#include "app.h"

#include <QApplication>
#include <memory>
#include "services/entity_manager.h"
#include "services/table_formatter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<EntityManager> em = std::make_shared<EntityManager>();
    TableFormatter formatter;
    App w = App(em);
    w.setFormatter(&formatter);
    w.run();
    w.show();
    return a.exec();
}
