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
    QHash<int, QString> opts;
    m_optionsLoader.fetchOptions(m_entityType, opts);
    QList<int> optIds = opts.keys();

    return opts.contains(data.toInt()) && opts.size() > 0;
}

void EntityChoiceDelegate::setEditorOptions(QComboBox *cb) const {
    QHash<int, QString> opts;
    m_optionsLoader.fetchOptions(m_entityType, opts);
    QHash<int, QString>::const_iterator i = opts.constBegin();
    while (i != opts.constEnd()) {
        qDebug() << "Adding option: " << i.value() << " id: " << i.key();

        cb->addItem(i.value(), i.key());
        i++;
    }
}
