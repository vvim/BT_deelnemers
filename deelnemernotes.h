#ifndef DEELNEMERNOTES_H
#define DEELNEMERNOTES_H

#include <QWidget>

namespace Ui {
class DeelnemerNotes;
}

class DeelnemerNotes : public QWidget
{
    Q_OBJECT

public:
    explicit DeelnemerNotes(QWidget *parent = 0);
    ~DeelnemerNotes();

private:
    Ui::DeelnemerNotes *ui;
};

#endif // DEELNEMERNOTES_H
