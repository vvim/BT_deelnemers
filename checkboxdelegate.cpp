#include "checkboxdelegate.h"

CheckboxDelegate::CheckboxDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget *CheckboxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QCheckBox *editor = new QCheckBox(parent);
    editor->setCheckable(true);
    return editor;
}

void CheckboxDelegate::setEditorData(QWidget * editor, const QModelIndex & index ) const
{
   // get value from the MODEL and bring it to the VIEW
   bool value = index.model()->data(index,Qt::EditRole).toBool();
   QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
   checkBox->setChecked(value);
}

void CheckboxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // get value from the VIEW and save it in the MODEL
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    bool value = checkBox->isChecked();
    model->setData(index, value, Qt::EditRole);
}

void CheckboxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // if you do not implement this, everytime you call the delegate, it will be floating in the upper left corner
    editor->setGeometry(option.rect);
}
