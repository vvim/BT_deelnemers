#include "completerdelegate.h"

#include <QLineEdit>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>


#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

CompleterDelegate::CompleterDelegate(QMap<QString, int> _deelnemers_map, QMap<int, QString> _id_map, QObject *parent)
:QItemDelegate(parent)
{
    deelnemers_map = _deelnemers_map;
    id_map = _id_map;
}


QWidget *CompleterDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
  QLineEdit* editor = new QLineEdit(parent);
  return editor;
}

void CompleterDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // get value from the MODEL and bring it to the VIEW
  QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
  int deelnemer_id = index.model()->data(index, Qt::EditRole).toInt();
    lineEdit->setText(id_map[deelnemer_id]);
}

void CompleterDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // get value from the VIEW and save it in the MODEL
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
  model->setData(index, deelnemers_map[lineEdit->text()], Qt::EditRole);
}

void CompleterDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    // if you do not implement this, everytime you call the delegate, it will be floating in the upper left corner
  editor->setGeometry(option.rect);
}

void CompleterDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyleOptionViewItemV4 myOption = option;

  int value = index.model()->index(index.row(),index.column()).data(Qt::EditRole).toInt();

  QString deelnemer = id_map[value];
  myOption.text = deelnemer;
  vvimDebug() << value << deelnemer;

  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}
