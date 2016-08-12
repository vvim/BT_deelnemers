#ifndef BTLISTVIEW_H
#define BTLISTVIEW_H

#include <QListView>

class BTListView : public QListView
{
    Q_OBJECT
public:
    explicit BTListView(QWidget *parent = 0);
    ~BTListView();

    void outputSelectedItems();
};

#endif // BTLISTVIEW_H
