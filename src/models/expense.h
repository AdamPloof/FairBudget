#ifndef EXPENSE_H
#define EXPENSE_H

#include <QAbstractTableModel>
#include <QString>
#include <QList>

class Expense : public QAbstractTableModel {
    Q_OBJECT

    using ExpenseLine = QList<QString>;

    public:
        explicit Expense(QObject *parent = nullptr);
        // ~Expense();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    private:
        QList<ExpenseLine> m_expenseLines;
};

#endif // EXPENSE_H