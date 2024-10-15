#ifndef COMBO_BOX_DELEGATE_H
#define COMBO_BOX_DELEGATE_H

#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QWidget>
#include <QObject>
#include <QHash>
#include <QString>
#include <QVariant>

#include "entity_options_loader.h"
#include "../entities/entity_interface.h"

class QComboBox;

class EntityChoiceDelegate : public QStyledItemDelegate {
public:
    EntityChoiceDelegate(
        EntityType et,
        EntityOptionsLoader optionsLoader,
        QObject *parent = nullptr
    );

    QWidget* createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index
    ) const override;

    void setEditorData(
        QWidget *editor,
        const QModelIndex &index
    ) const override;

    void updateEditorGeometry(
        QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index
    ) const override;

    void setModelData(
        QWidget *editor,
        QAbstractItemModel *model,
        const QModelIndex &index
    ) const override;

private:
    bool supports(QVariant data) const;
    void setEditorOptions(QComboBox *cb) const;

    EntityType m_entityType;
    EntityOptionsLoader m_optionsLoader;
};

#endif // COMBO_BOX_DELEGATE_H
