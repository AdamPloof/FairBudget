#ifndef PAYMENT_MODEL_H
#define PAYMENT_MODEL_H

#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QList>
#include <memory>

class EntityInterface;
class EntityManager;

class PaymentModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        explicit PaymentModel(
            std::shared_ptr<EntityManager> em,
            QObject *parent = nullptr
        );
        // ~PaymentModel();

        void load();
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
        void addPayment(std::shared_ptr<EntityInterface> payment);
        void removePayment(std::shared_ptr<EntityInterface> payment);
        // void refreshAmounts();

    // signals:
    //     void expensesChanged();

    private:
        QList<std::shared_ptr<EntityInterface>> m_payments;
        std::shared_ptr<EntityManager> m_entityManager;
};

#endif // PAYMENT_MODEL_H
