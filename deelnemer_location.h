#ifndef DEELNEMER_LOCATION_H
#define DEELNEMER_LOCATION_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class deelnemer_location;
}

class DeelnemerLocation : public QWidget
{
    Q_OBJECT

public:
    explicit DeelnemerLocation(QString latitude, QString longitude, int zoom = 15, QWidget *parent = 0);
    ~DeelnemerLocation();

private:
    Ui::deelnemer_location *ui;
    QSettings *settings;
};

#endif // DEELNEMER_LOCATION_H
