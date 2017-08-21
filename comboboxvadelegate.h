#ifndef COMBOBOXVADELEGATE_H
#define COMBOBOXVADELEGATE_H

#include <string>
#include <vector>
#include <QDebug>

#include <QItemDelegate>

class QModelIndex;
class QWidget;
class QVariant;

class ComboBoxVADelegate : public QItemDelegate
{
Q_OBJECT
public:
  ComboBoxVADelegate(QObject *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
  QMap<int, QString> Items;

};
#endif
