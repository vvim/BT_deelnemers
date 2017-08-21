#include "comboboxvadelegate.h"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>


#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

ComboBoxVADelegate::ComboBoxVADelegate(QObject *parent)
:QItemDelegate(parent)
{
  Items.push_back("AANBOD");
  Items.push_back("VRAAG");
}


QWidget *ComboBoxVADelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
  QComboBox* editor = new QComboBox(parent);
  for(unsigned int i = 0; i < Items.size(); ++i)
    {
        editor->addItem(Items[i].c_str());
    }
  return editor;
}

void ComboBoxVADelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  int value = index.model()->data(index, Qt::EditRole).toUInt();
  comboBox->setCurrentIndex(value);
}

void ComboBoxVADelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

void ComboBoxVADelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

void ComboBoxVADelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyleOptionViewItemV4 myOption = option;
  QString text = Items[index.row()].c_str();

  myOption.text = text;

  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}
