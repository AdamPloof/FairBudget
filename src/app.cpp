#include <QString>
#include <QDebug>
#include <vector>

#include "app.h"
#include "./ui_app.h"

App::App(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::App)
{
    ui->setupUi(this);
    loadDb();
}

App::~App()
{
    delete ui;
}

void App::loadDb() {
    m_entityManager.openDb();

    std::vector<QString> fields;
    fields.push_back("id");
    fields.push_back("description");
    fields.push_back("amount");

    QString entity = "expense";
    EntityQueryParams params;
    params.entityName = entity;
    params.fields = fields;

    QString q("SELECT * FROM expense");
    m_entityManager.fetchRecords(params);
}
