#ifndef NotasSortFilterProxyModel_H
#define NotasSortFilterProxyModel_H

#define NOTA_TIMESTAMP 1


#include <QSortFilterProxyModel>
#include <QDate>

class NotasSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT;

public:
    NotasSortFilterProxyModel(QObject *parent = 0);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // NotasSortFilterProxyModel_H
