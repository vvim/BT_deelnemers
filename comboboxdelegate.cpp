#include "comboboxdelegate.h"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>


#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

ComboBoxDelegate::ComboBoxDelegate(std::vector<std::string> _items, QObject *parent)
:QItemDelegate(parent)
{
    Items = _items;
}


QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
  QComboBox* editor = new QComboBox(parent);
  for(unsigned int i = 0; i < Items.size(); ++i)
    {
        editor->addItem(Items[i].c_str());
    }
  return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // get value from the MODEL and bring it to the VIEW
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  int value = index.model()->data(index, Qt::EditRole).toUInt();
  comboBox->setCurrentIndex(value);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // get value from the VIEW and save it in the MODEL
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    // if you do not implement this, everytime you call the delegate, it will be floating in the upper left corner
  editor->setGeometry(option.rect);
}

void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyleOptionViewItemV4 myOption = option;

  //QString text = Items[index.model()->index(index.row(),index.column()).data(Qt::EditRole).toInt()];
  QString text = QString::fromStdString(Items[index.model()->data(index, Qt::EditRole).toInt()]);
  myOption.text = text;

  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}


/** *****************************
 * for more information:
 see https://stackoverflow.com/questions/15442009/how-to-set-a-delegate-for-a-single-cell-in-qt-item-view
 see http://www.qtforum.org/article/14425/delegate-only-in-one-column.html
 see https://doc.qt.io/archives/qq/qq24-delegates.html
 see www.qtcentre.org/threads/15249-Different-delegates-to-different-Columns-of-QTableView
 see http://programmingexamples.net/wiki/Qt/Delegates/ComboBoxDelegate
** *****************************/
