#ifndef DEELNEMERBHOVERZICHT_H
#define DEELNEMERBHOVERZICHT_H

#include <QDialog>
#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>
#include "sdeelnemermarker.h"

namespace Ui {
class DeelnemerBHOverzicht;
}

class DeelnemerBHOverzicht : public QDialog
{
    Q_OBJECT

public:
    explicit DeelnemerBHOverzicht(SDeelnemerMarker _deelnemer, QWidget *parent = 0);
    ~DeelnemerBHOverzicht();

private slots:
    void on_pushButton_set_max_lines_clicked();
    void print(QPrinter* printer);

    void on_pushButton_print_clicked();

private:
    Ui::DeelnemerBHOverzicht *ui;
    int deelnemer_id, overzicht_max_lines;
    void showOverzicht();
    QScopedPointer<QPrinter> m_printer;
};

#endif // DEELNEMERBHOVERZICHT_H
