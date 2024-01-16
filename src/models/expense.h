#ifndef EXPENSE_H
#define EXPENSE_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>
#include "model_interface.h"

class Expense : public QAbstractTableModel, public ModelInterface {
    Q_OBJECT

    using ExpenseLine = QList<QString>;

    public:
        explicit Expense(QObject *parent = nullptr);
        // ~Expense();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        QString name() override;
        std::vector<QString> fields() override;
        void addRow(ExpenseLine row) override;

    private:
        QList<ExpenseLine> m_expenseLines;
        QString m_name;
        std::vector<QString> m_fields;
};

#endif // EXPENSE_H