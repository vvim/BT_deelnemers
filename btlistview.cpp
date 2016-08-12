#include "btlistview.h"
#include <QDebug>

BTListView::BTListView(QWidget *widget ) :
    QListView(widget)
{
    m_selectedItemsList = "-1";
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

void BTListView::setSelectedItemsList(const QString &selecteditems)
{
    m_selectedItemsList = selecteditems;

    // 1) deselect all items from the listview
    this->clearSelection();

    // 2) select the IDs found in m_selectedItemsList
    /**
      * [TODO:]
      *    a. check if the IDs in m_selectedItemsList are unique
      *             (not necessary, does it really matter?)
      *
      *    b. check if the IDs in m_selectedItemsList are existing IDs of t_deelnemer_doelgroep or t_deelnemer_domein
      *             (if we search an ID in the model of listview, and we cannot find it, we simply do not select an item, is that enough?)
      *
      *    c. we should select the row which has the same ID as in m_selectedItemsList , this is not the same as the rownumber!
      *
    **/

    // m_selectedItemsList is QString containing the selected items from the QListView, seperated by ","

    // see http://stackoverflow.com/a/14175044
    QStringList list = selecteditems.split(", ",QString::SkipEmptyParts);

    qDebug() << this->objectName() << "selected items:" << list.count() << "row count:" << this->model()->rowCount();

    for ( int i = 0 ; i < this->model()->rowCount() ; ++i )
    {
        if( list.contains(this->model()->index( i, 0 ).data().toString()) )
        {
            qDebug() << "found ID" << this->model()->index( i, 0 ).data().toString();
            this->setCurrentIndex(this->model()->index( i, 1));
        }
    }
}

QString BTListView::selectedItemsList() const
{
    return m_selectedItemsList;
}
