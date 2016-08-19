#ifndef DEELNEMER_LOCATION_H
#define DEELNEMER_LOCATION_H

#include <QWidget>
#include <QSettings>
#include <QSqlRelationalTableModel>
#include "sdeelnemermarker.h"

namespace Ui {
class deelnemer_location;
}

class DeelnemerLocation : public QWidget
{
    Q_OBJECT

public:
    explicit DeelnemerLocation(SDeelnemerMarker *_deelnemer, QSqlRelationalTableModel *_model_deelnemers, int _zoom = 15, QWidget *parent = 0);
    ~DeelnemerLocation();

private slots:
    void on_pushButton_showAllDeelnemers_clicked();

private:
    Ui::deelnemer_location *ui;
    QSettings *settings;
    SDeelnemerMarker *deelnemerMarker;
    QSqlRelationalTableModel *model_deelnemers;
    int zoom;
};

#endif // DEELNEMER_LOCATION_H
