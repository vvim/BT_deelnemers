#include "notassortfilterproxymodel.h"
#include <QLocale>

NotasSortFilterProxyModel::NotasSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    QLocale::setDefault(QLocale(QLocale::Dutch, QLocale::Belgium));
}

bool NotasSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    if (leftData.type() == QVariant::DateTime)
    {
            return leftData.toDateTime() < rightData.toDateTime();
    }
    else if (leftData.type() == QVariant::Int)
    {
        return leftData.toInt() < rightData.toInt();
    }
    else if (leftData.type() == QVariant::Double)
    {
        return leftData.toDouble() < rightData.toDouble();
    }
    else
    {
        QString leftString = leftData.toString();
        QString rightString = rightData.toString();
        return QString::localeAwareCompare(leftString, rightString) < 0;
    }
}

QVariant NotasSortFilterProxyModel::data(const QModelIndex &index, int role) const
{
    int col = index.column();

    switch(role)
    {

    case Qt::DisplayRole:
        if(col == NOTA_TIMESTAMP)
            return QLocale().toString(QSortFilterProxyModel::data(index,role).toDate(),"d MMM yyyy");
        break;
    }

    return QSortFilterProxyModel::data(index,role);
}
