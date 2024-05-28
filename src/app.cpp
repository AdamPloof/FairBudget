#include <QString>
#include <QDebug>
#include <vector>
#include <QTableView>

#include "app.h"
#include "./ui_app.h"

App::App(std::shared_ptr<EntityManager> em, QWidget *parent)
    : QMainWindow(parent),
    m_entityManager(em),
    ui(new Ui::App)
{
    ui->setupUi(this);
}

App::~App()
{
    delete ui;
}

void App::run() {
    loadDb();
}

void App::loadDb() {
    m_entityManager->openDb();
}

void App::setFormatter(TableFormatter* formatter) {
    m_formatter = formatter;
}

void App::loadTables() {

}

void App::loadForms() {

}

void App::formatTable(QTableView* tbl) {

}

void App::on_addExpenseBtn_clicked() {
    
}

