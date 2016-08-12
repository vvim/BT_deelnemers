#include "btlistview.h"
#include <QDebug>

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

BTListView::BTListView(QWidget *widget ) :
    QListView(widget)
{
    m_selectedItemsList = "-1";
}


BTListView::~BTListView()
{

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

    vvimDebug() << this->objectName() << "selected items:" << list.count() << "row count:" << this->model()->rowCount();

    for ( int i = 0 ; i < this->model()->rowCount() ; ++i )
    {
        if( list.contains(this->model()->index( i, 0 ).data().toString()) )
        {
            vvimDebug() << "found ID" << this->model()->index( i, 0 ).data().toString();
            this->model()->setData(this->model()->index( i, 1 ), Qt::Checked, Qt::CheckStateRole);
        }
        else
            this->model()->setData(this->model()->index( i, 1 ), Qt::Unchecked, Qt::CheckStateRole);
    }
}

QString BTListView::selectedItemsList() const
{
    BTSqlTableModel* model = (BTSqlTableModel*) this->model();

    return model->getCheckedItems().join(", ");
}
