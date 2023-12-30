#ifndef OWED_TABLE_H
#define OWED_TABLE_H

#include <QWidget>

namespace Ui {
class OwedTable;
}

class OwedTable : public QWidget
{
    Q_OBJECT

public:
    explicit OwedTable(QWidget *parent = nullptr);
    ~OwedTable();

private:
    Ui::OwedTable *ui;
};

#endif // OWED_TABLE_H
