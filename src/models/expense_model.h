#ifndef EXPENSE_MODEL_H
#define EXPENSE_MODEL_H

#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QString>
#include <QList>
#include <QLocale>
#include <memory>

class EntityInterface;
class EntityManager;

class ExpenseModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        explicit ExpenseModel(
            std::shared_ptr<EntityManager> em,
            QObject *parent = nullptr
        );
        // ~ExpenseModel();

        void load();
        void setLocale(std::shared_ptr<QLocale> locale);
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        Qt::ItemFlags flags(const QModelIndex &index) const override;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
        bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
        std::shared_ptr<EntityInterface> getRow(int row);

    public slots:
        void addExpense(std::shared_ptr<EntityInterface> expense);
        void removeExpense(std::shared_ptr<EntityInterface> expense);

    private:
        QList<std::shared_ptr<EntityInterface>> m_expenses;
        std::shared_ptr<EntityManager> m_entityManager;
        std::shared_ptr<QLocale> m_locale;
};

#endif // EXPENSE_MODEL_H
