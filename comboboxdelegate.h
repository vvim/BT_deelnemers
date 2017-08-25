#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <string>
#include <vector>
#include <QDebug>

#include <QItemDelegate>

class QModelIndex;
class QWidget;
class QVariant;

class ComboBoxDelegate : public QItemDelegate
{
Q_OBJECT
public:
  ComboBoxDelegate(std::vector<std::string> _items, QObject *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

  // might add later: QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
  // return QSize depending on longest string in Items ?

private:
  std::vector<std::string> Items;

};
#endif
