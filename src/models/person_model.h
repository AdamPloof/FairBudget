#ifndef PERSON_MODEL_H
#define PERSON_MODEL_H

#include <QAbstractTableModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QList>
#include <memory>

class EntityInterface;
class EntityManager;

class PersonModel : public QAbstractTableModel {
    Q_OBJECT

    public:
        explicit PersonModel(
            std::shared_ptr<EntityManager> em,
            QObject *parent = nullptr
        );
        // ~PersonModel();

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
        void addPerson(std::shared_ptr<EntityInterface> person);
        void removePerson(std::shared_ptr<EntityInterface> person);

    private:
        void buildPerson(QSqlQuery *q);

        QList<std::shared_ptr<EntityInterface>> m_persons;
        std::shared_ptr<EntityManager> m_entityManager;
};

#endif // PERSON_MODEL_H
