#ifndef PAYMENT_MODEL_H
#define PAYMENT_MODEL_H

#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QString>
#include <QList>
#include <QLocale>
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
        void addPayment(std::shared_ptr<EntityInterface> payment);
        void removePayment(std::shared_ptr<EntityInterface> payment);

        /**
         * Called when a related expense or person is removed that had payments
         * associated with it that also need to be removed.
         * 
         * Since cascade removal is handled at the level of the entity manager, handling removal
         * in this slot involves removing the orphaned rows from the model.
         */
        void cascadeRemove(QList<int> ids);

    private:
        QList<std::shared_ptr<EntityInterface>> m_payments;
        std::shared_ptr<EntityManager> m_entityManager;
        std::shared_ptr<QLocale> m_locale;
};

#endif // PAYMENT_MODEL_H
