#include "btlistview.h"
#include <QDebug>

BTListView::BTListView(QWidget *widget ) :
    QListView(widget)
{

}


BTListView::~BTListView()
{

}

void BTListView::outputSelectedItems()
{
    // see http://stackoverflow.com/questions/11246022/how-to-get-qstring-from-qlistview-selected-item-in-qt

    QStringList list;
    foreach(const QModelIndex &index, this->selectedIndexes())
        list.append(QString::number(index.row()));

    /** when somebody called list.SelectAll() , the function `selectedIndexes()`
     *  will return the indexes (1,0), (3,0), (5,0), (7,0), (9,0),
     *  and (0,1), (1,1), (2,1), (3,1), (4,1), (5,1), (6,1), (7,1), (8,1), (9,1)
     *
     *  I have no idea why only the odd rows get a double return...
     *
     *  As we are only interested in the rownumbers (ids), we do not need to know the selected column
     *
     *  example output after a list.SelectAll() , or when the user does a Ctrl+A
     *
     *  code:
     *  //    qDebug() << "count:" << this->selectedIndexes().count() << list.join(",");
     *  //    qDebug() << this->selectedIndexes();
     *
     *  output:
     *    count: 15 "1,3,5,7,9,0,1,2,3,4,5,6,7,8,9"
     *    (QModelIndex(1,0,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(3,0,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(5,0,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(7,0,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(9,0,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(0,1,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(1,1,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(2,1,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(3,1,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(4,1,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(5,1,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(6,1,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(7,1,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(8,1,0x0,QSqlRelationalTableModel(0x187fae0) )
     *     QModelIndex(9,1,0x0,QSqlRelationalTableModel(0x187fae0) )  )
    **/


    list.removeDuplicates();
    list.sort();

    // actually we should return modelindex(list[i],0) to get the ID , not simply the ROWnr
    qDebug() << "selected" << list.join(",") << "count: " << list.count();
}
