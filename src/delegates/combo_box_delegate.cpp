#include <QList>
#include <QComboBox>
#include <QSqlQuery>
#include <QDebug>

#include "combo_box_delegate.h"
#include "../forms/add_person_form.h"

ComboBoxDelegate::ComboBoxDelegate(
    QObject *parent
) : QStyledItemDelegate(parent) {
    fetchOptions();
}

QWidget* ComboBoxDelegate::createEditor(
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

void ComboBoxDelegate::setEditorData(
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
        qDebug() << "Setting model data for income_period: " << cb->currentData().toInt();
        model->setData(index, cb->currentData());
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

bool ComboBoxDelegate::supports(QVariant data) const {
    QList<int> opts = m_options.keys();

    return opts.contains(data.toInt()) && m_options.size() > 0;
}

// TODO: make this more generalizable. Maybe make a "ChoiceFieldDelegate"
void ComboBoxDelegate::fetchOptions() {
    QSqlQuery q;
    q.prepare("SELECT id, label FROM income_period");
    if (!q.exec()) {
        qDebug() << "Could not fetch income periods for person table";
    }

    while (q.next()) {
        m_options.insert(q.value(0).toInt(), q.value(1).toString());
    }
}

void ComboBoxDelegate::setEditorOptions(QComboBox *cb) const {
    QHash<int, QString>::const_iterator i = m_options.constBegin();
    while (i != m_options.constEnd()) {
        qDebug() << "Adding option: " << i.value() << " id: " << i.key();

        cb->addItem(i.value(), i.key());
        i++;
    }
}
