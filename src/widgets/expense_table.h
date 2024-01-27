#ifndef EXPENSE_TABLE_H
#define EXPENSE_TABLE_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include "../models/expense.h"
#include "../models/model_interface.h"
#include "table_widget_interface.h"

class ExpenseTable : public QAbstractTableModel, public TableWidgetInterface {
    Q_OBJECT

    public:
        explicit ExpenseTable(QObject *parent = nullptr);
        // ~ExpenseTable();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        void addRow(std::shared_ptr<ModelInterface> row) override;

    private:
        QList<std::shared_ptr<ModelInterface>> m_expenses;
};

#endif // EXPENSE_TABLE_H
 