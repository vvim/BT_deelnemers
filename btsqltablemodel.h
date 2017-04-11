#ifndef BTSQLTABLEMODEL_H
#define BTSQLTABLEMODEL_H

// based on example by WalletFox.com : http://www.walletfox.com/course/qtcheckablelist.php

#include <QSqlTableModel>
#include <QSet>

class BTSqlTableModel : public QSqlTableModel
{
public:
    explicit BTSqlTableModel(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    virtual ~BTSqlTableModel();

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role);
    QStringList getCheckedItems();

private:
    QSet<QModelIndex> checkedItems;
    static bool intStringSort( const QString &s1, const QString &s2 );
};

#endif // BTSQLTABLEMODEL_H
