#ifndef PERSON_TABLE_H
#define PERSON_TABLE_H

#include <QWidget>

namespace Ui {
class PersonTable;
}

class PersonTable : public QWidget
{
    Q_OBJECT

public:
    explicit PersonTable(QWidget *parent = nullptr);
    ~PersonTable();

private:
    Ui::PersonTable *ui;
};

#endif // PERSON_TABLE_H
