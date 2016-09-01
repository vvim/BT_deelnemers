#ifndef NOTASLISTVIEW_H
#define NOTASLISTVIEW_H

#include <QListView>

class NotasListView : public QListView
{
    Q_OBJECT
public:
    explicit NotasListView(QWidget *parent = 0);
    ~NotasListView();
};

#endif // NOTASLISTVIEW_H
