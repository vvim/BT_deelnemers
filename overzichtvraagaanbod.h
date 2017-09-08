#ifndef OVERZICHTVRAAGAANBOD_H
#define OVERZICHTVRAAGAANBOD_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include <QMessageBox>

namespace Ui {
class OverzichtVraagAanbod;
}

class OverzichtVraagAanbod : public QDialog
{
    Q_OBJECT

public:
    explicit OverzichtVraagAanbod(QSqlRelationalTableModel *_model_vraag_aanbod_overzicht, QSqlRelationalTableModel *_model_deelnemers, QWidget *parent = 0);
    ~OverzichtVraagAanbod();

private slots:
    void on_saveButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::OverzichtVraagAanbod *ui;
    QSqlRelationalTableModel *model_vraag_aanbod_overzicht;
    OverzichtVraagAanbod *vraag_aanbod;
    QMap<QString, int> deelnemers_map;
    QMap<int, QString> id_map;
    void feedbackSuccess(QString message);
    void feedbackWarning(QString message);
    void feedbackNeutral(QString message);
};

#endif // OVERZICHTVRAAGAANBOD_H
