#ifndef BTLISTVIEW_H
#define BTLISTVIEW_H

#include <QListView>
#include "btsqltablemodel.h"

class BTListView : public QListView
{
    Q_OBJECT
    Q_PROPERTY(QString selectedItemsList READ selectedItemsList WRITE setSelectedItemsList)
public:
    explicit BTListView(QWidget *parent = 0);
    ~BTListView();

    void outputSelectedItems();

    void setSelectedItemsList(const QString &selecteditems);
    QString selectedItemsList() const;

private:
    QString m_selectedItemsList; //http://doc.qt.io/qt-4.8/properties.html
};

#endif // BTLISTVIEW_H
