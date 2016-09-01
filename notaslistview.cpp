#include "notaslistview.h"
#include <QDebug>

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

NotasListView::NotasListView(QWidget *widget ) :
    QListView(widget)
{
}


NotasListView::~NotasListView()
{

}
