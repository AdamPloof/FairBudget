#include <QList>
#include <QComboBox>
#include <QDebug>

#include "entity_choice_delegate.h"
#include "../forms/add_person_form.h"

EntityChoiceDelegate::EntityChoiceDelegate(
    EntityType et,
    EntityOptionsLoader optionsLoader,
    QObject *parent
) : 
    QStyledItemDelegate(parent),
    m_entityType(et),
    m_optionsLoader(optionsLoader)
{
    if (!m_optionsLoader.fetchOptions(m_entityType, m_options)) {
        throw std::runtime_error("Could not load options for table editor");
    }
}

QWidget* EntityChoiceDelegate::createEditor(
    QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index
) const {
    if (supports(index.data(Qt::EditRole))) {
        QComboBox *editor = new QComboBox(parent);
        editor->setAutoFillBackground(true);
        setEditorOptions(editor);

        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void EntityChoiceDelegate::setEditorData(
    QWidget *editor,
    const QModelIndex &index
) const {
    if (supports(index.data(Qt::EditRole))) {
        QComboBox *cbEditor = qobject_cast<QComboBox *>(editor);
        int idx = cbEditor->findData(index.data(Qt::EditRole));
        if (idx == -1) {
            return;
        }

        cbEditor->setCurrentIndex(idx);
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void EntityChoiceDelegate::updateEditorGeometry(
    QWidget *editor,
    const QStyleOptionViewItem &option,
    const QModelIndex &index
) const {
    editor->setGeometry(option.rect);
}

void EntityChoiceDelegate::setModelData(
    QWidget *editor,
    QAbstractItemModel *model,
    const QModelIndex &index
) const {
    if (QComboBox *cb = qobject_cast<QComboBox *>(editor)) {
        qDebug() << "Setting model data for income_period: " << cb->currentData().toInt();
        model->setData(index, cb->currentData());
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

bool EntityChoiceDelegate::supports(QVariant data) const {
    QList<int> opts = m_options.keys();

    return opts.contains(data.toInt()) && m_options.size() > 0;
}

void EntityChoiceDelegate::setEditorOptions(QComboBox *cb) const {
    QHash<int, QString>::const_iterator i = m_options.constBegin();
    while (i != m_options.constEnd()) {
        qDebug() << "Adding option: " << i.value() << " id: " << i.key();

        cb->addItem(i.value(), i.key());
        i++;
    }
}
