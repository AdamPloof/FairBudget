#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "person_model.h"
#include "../services/entity_manager.h"
#include "../entities/person.h"
#include "../entities/entity_interface.h"

PersonModel::PersonModel(
    std::shared_ptr<EntityManager> em,
    QObject *parent
) : m_entityManager(em) {

}

// PersonModel::~PersonModel() {
    
// }

void PersonModel::load() {
    QSqlQuery q = QSqlQuery("SELECT id, name, income, income_period FROM person");
    while (q.next()) {
        std::shared_ptr<Person> person = std::make_shared<Person>(Person());
        person->setData("id", q.value(0).toInt());
        person->setData("name", q.value(1).toString());
        person->setData("income", q.value(2).toFloat());
        person->setData("income_period", q.value(3).toString());
        m_persons.push_back(person);

        qDebug() << "Load person: " << q.value(0).toString();
    }

    qDebug() << "Person count: " << m_persons.count();
}

int PersonModel::rowCount(const QModelIndex &parent) const {
    return m_persons.count();
}

int PersonModel::columnCount(const QModelIndex &parent) const {
    return Person::fields.size();
}

QVariant PersonModel::data(const QModelIndex &index, int role) const {
    if (
        (role == Qt::DisplayRole || role == Qt::EditRole)
        && !m_persons.isEmpty()
    ) {
        QString field = Person::fields.at(index.column());

        return m_persons[index.row()]->getData(field);
    }

    return QVariant();
}

QVariant PersonModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return Person::fields[section];
    }

    return QVariant();
}

Qt::ItemFlags PersonModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    } else if (index.column() == 0) {
        // ID is always column 0 and should be read-only
        return QAbstractItemModel::flags(index) | Qt::ItemIsSelectable;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool PersonModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        QString field = Person::fields.at(index.column());
        m_persons[index.row()]->setData(field, value);
        m_entityManager->update(m_persons[index.row()]);
        
        emit dataChanged(index, index, {role});

        return true;
    }

    return false;
}

bool PersonModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row > rowCount() || count <= 0) {
        return false;
    }

    beginInsertRows(parent, row, row + count - 1);
    m_persons.push_back(nullptr);
    endInsertRows();

    return true;
}

bool PersonModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (row < 0 || row > rowCount() || count <= 0) {
        return false;
    }

    beginRemoveRows(parent, row, row + count - 1);

    for (int i = 0; i < count; i++) {
        m_persons.removeAt(row);
    }

    endRemoveRows();

    return true;
}

void PersonModel::addPerson(std::shared_ptr<EntityInterface> person) {
    QSqlQuery q;
    q.prepare("INSERT INTO person (name, income, income_period) VALUES (:name, :income, :income_period) RETURNING id");
    q.bindValue(":name", person->getData("name").toString());
    q.bindValue(":income", person->getData("income").toFloat());
    q.bindValue(":income_period", person->getData("income_period").toString());

    if (q.exec() == false) {
        // TODO: handle error
        qDebug() << "Failed to insert person: " << person->getData("name");
        return;
    }
    q.next();
    qDebug() << "Inserted: " << q.value(0).toInt();

    person->setData("id", q.value(0));

    if (insertRows(rowCount(), 1)) {
        m_persons.replace(rowCount() - 1, person);
    }
}

void PersonModel::removePerson(std::shared_ptr<EntityInterface> person) {
    QSqlQuery q;
    q.prepare("DELETE FROM person WHERE id = :id");
    q.bindValue(":id", person->getId());
    if (q.exec() == false) {
        // TODO: handle error
        qDebug() << "Failed to delete person: " << person->getId();
        return;
    }

    qDebug() << "Delete person: " << person->getId();
    int row = -1;
    for (int i = 0; i < m_persons.size(); i++) {
        if (m_persons.at(i) == person) {
            row = i;
            break;
        }
    }

    if (row != -1) {
        removeRows(row, 1);
    }
}

std::shared_ptr<EntityInterface> PersonModel::getRow(int row) {
    if (row < 0 || row > m_persons.size() - 1) {
        return nullptr;
    }

    return m_persons.at(row);
}
