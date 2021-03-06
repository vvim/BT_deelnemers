#include <QColor>
#include <QDebug>
#include "btsqltablemodel.h"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

// based on example by WalletFox.com : http://www.walletfox.com/course/qtcheckablelist.php


BTSqlTableModel::BTSqlTableModel(QObject *parent, QSqlDatabase db) :
    QSqlTableModel(parent,db)
{

}

BTSqlTableModel::~BTSqlTableModel()
{

}

Qt::ItemFlags BTSqlTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QSqlTableModel::flags(index);
    if (index.isValid())
    {
        return defaultFlags | Qt::ItemIsUserCheckable;
    }
    return defaultFlags;
}



QVariant BTSqlTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::CheckStateRole)
        return checkedItems.contains(index) ?
                    Qt::Checked : Qt::Unchecked;

    else if(role == Qt::BackgroundColorRole)
        return checkedItems.contains(index) ?
                    QColor("#ffffb2") : QColor("#ffffff");

    return QSqlTableModel::data(index, role);
}

bool BTSqlTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if(!index.isValid() || role != Qt::CheckStateRole)
        return false;

    if(value == Qt::Checked)
        checkedItems.insert(index);
    else
        checkedItems.remove(index);

    emit dataChanged(index, index);
    return true;
}

QStringList BTSqlTableModel::getCheckedItems()
{
    QStringList checked_ids;
    foreach(QModelIndex index, checkedItems)
    {
        checked_ids.append(index.sibling(index.row(),0).data().toString());
    }
    qSort( checked_ids.begin(), checked_ids.end(), intStringSort );
    vvimDebug() << checked_ids.count() << "items aangevinkt";
    vvimDebug() << checked_ids;
    return checked_ids;
}

bool BTSqlTableModel::intStringSort( const QString &s1, const QString &s2 )
{
            return( s1.toInt() < s2.toInt() );
}
