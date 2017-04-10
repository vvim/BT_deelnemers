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

signals:
    void addNewIndividu(QString,QString,QString,QString,QString,QString,QString);

private slots:
    void on_buttonBox_accepted();

    void on_rb_individu_toggled(bool checked);

private:
    Ui::NewDeelnemerIndividu *ui;
};

#endif // NEWDEELNEMERINDIVIDU_H
