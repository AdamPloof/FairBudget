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

class QComboBox;

class ComboBoxDelegate : public QStyledItemDelegate {
public:
    ComboBoxDelegate(QObject *parent = nullptr);

    void setOptions(QHash<QString, QVariant> opts);

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

    QHash<QString, QVariant> m_options;
};

#endif // COMBO_BOX_DELEGATE_H
