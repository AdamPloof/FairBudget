#include <QList>
#include <QString>
#include <QSqlError>
#include <QDebug>

#include "person_model.h"
#include "../services/entity_manager.h"
#include "../entities/entity_interface.h"
#include "../entities/person.h"
#include "../entities/income_period.h"
#include "../services/utils.h"

using namespace FairBudget::utils;

PersonModel::PersonModel(
    std::shared_ptr<EntityManager> em,
    QObject *parent
) : m_entityManager(em) {

}

// PersonModel::~PersonModel() {
    
// }

void PersonModel::load() {
    if (!m_entityManager->isReady()) {
        throw std::runtime_error("EntityManager must be ready when loading models");
    }

    m_persons = m_entityManager->findAll(EntityType::PERSON);
}

void PersonModel::setLocale(std::shared_ptr<QLocale> locale) {
    m_locale = locale;
}

int PersonModel::rowCount(const QModelIndex &parent) const {
    return m_persons.count();
}

int PersonModel::columnCount(const QModelIndex &parent) const {
    return Person::fields.size();
}

QVariant PersonModel::data(const QModelIndex &index, int role) const {
    if (m_persons.isEmpty()) {
        return QVariant();
    }

    if (role == Qt::EditRole) {
        QString field = Person::fields.at(index.column());
        return m_persons[index.row()]->getData(field, role);
    } else if (role == Qt::DisplayRole) {
        QString field = Person::fields.at(index.column());
        QVariant val = m_persons[index.row()]->getData(field, role);
        if (field == "income" && m_locale != nullptr) {
            return m_locale->toCurrencyString(val.toDouble());
        }

        return val;
    }

    return QVariant();
}

QVariant PersonModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        return sentenceCase(Person::fields[section]);
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

/**
 * When updating the data for the income_period field, the value will be the id
 * of income_period entity.
 */
bool PersonModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        QString field = Person::fields.at(index.column());
        if (field == "income_period") {
            std::shared_ptr<Person> p = std::dynamic_pointer_cast<Person>(m_persons[index.row()]);
            if (!p) {
                return false;
            }
            
            std::shared_ptr<IncomePeriod> ip = m_entityManager->find<IncomePeriod>(value.toInt());
            p->setIncomePeriod(ip);
        } else {
            m_persons[index.row()]->setData(field, value);
        }

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
    if (!m_entityManager->persist(person)) {
        return;
    }

    if (insertRows(rowCount(), 1)) {
        m_persons.replace(rowCount() - 1, person);
    }
}

void PersonModel::removePerson(std::shared_ptr<EntityInterface> person) {
    if (!m_entityManager->remove(person)) {
        return;
    }

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
