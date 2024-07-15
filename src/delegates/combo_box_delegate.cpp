#include "combo_box_delegate.h"
#include "../forms/add_person_form.h"

#include <QList>
#include <QComboBox>
#include <QPainter>
#include <QDebug>

ComboBoxDelegate::ComboBoxDelegate(
    QObject *parent
) : QStyledItemDelegate(parent) {}

void ComboBoxDelegate::setOptions(QHash<QString, QVariant> opts) {
    m_options = opts;
}

QWidget* ComboBoxDelegate::createEditor(
    QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index
) const {
    if (supports(index.data())) {
        QComboBox *editor = new QComboBox(parent);
        editor->setAutoFillBackground(true);
        setEditorOptions(editor);

        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ComboBoxDelegate::setEditorData(
    QWidget *editor,
    const QModelIndex &index
) const {
    if (supports(index.data())) {
        QComboBox *cbEditor = qobject_cast<QComboBox *>(editor);
        int idx = cbEditor->findData(index.data());
        if (idx == -1) {
            return;
        }

        cbEditor->setCurrentIndex(idx);
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void ComboBoxDelegate::updateEditorGeometry(
    QWidget *editor,
    const QStyleOptionViewItem &option,
    const QModelIndex &index
) const {
    editor->setGeometry(option.rect);
}

void ComboBoxDelegate::setModelData(
    QWidget *editor,
    QAbstractItemModel *model,
    const QModelIndex &index
) const {
    if (QComboBox *cb = qobject_cast<QComboBox *>(editor)) {
        model->setData(index, cb->currentData());
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

bool ComboBoxDelegate::supports(QVariant data) const {
    QList<QVariant> opts = m_options.values();

    return opts.contains(data) && m_options.size() > 0;
}

void ComboBoxDelegate::setEditorOptions(QComboBox *cb) const {
    QHash<QString, QVariant>::const_iterator i = m_options.constBegin();
    while (i != m_options.constEnd()) {
        cb->addItem(i.key(), i.value());
        i++;
    }
}
