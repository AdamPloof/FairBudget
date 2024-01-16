#include "table_formatter.h"

void TableFormatter::format(QTableView* table) {
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
