#ifndef NEWDEELNEMERINDIVIDU_H
#define NEWDEELNEMERINDIVIDU_H

#include <QDialog>

namespace Ui {
class NewDeelnemerIndividu;
}

class NewDeelnemerIndividu : public QDialog
{
    Q_OBJECT

public:
    explicit NewDeelnemerIndividu(QWidget *parent = 0);
    ~NewDeelnemerIndividu();

private:
    Ui::NewDeelnemerIndividu *ui;
};

#endif // NEWDEELNEMERINDIVIDU_H
