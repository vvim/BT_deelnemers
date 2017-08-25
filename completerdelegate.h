#ifndef COMPLETERDELEGATE_H
#define COMPLETERDELEGATE_H

#include <string>
#include <vector>
#include <QDebug>
#include <QSqlRelationalTableModel>
#include "mylineedit.h"

#include <QItemDelegate>

class QModelIndex;
class QWidget;
class QVariant;

class CompleterDelegate : public QItemDelegate
{
Q_OBJECT
public:
  CompleterDelegate(QSqlRelationalTableModel *_model_deelnemers, QObject *parent = 0);
  void loadCompleter();

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QMap<QString, int> deelnemers_map;
    QMap<int, QString> id_map;
    MyCompleter *completer;
    QSqlRelationalTableModel *model_deelnemers;
};
#endif
