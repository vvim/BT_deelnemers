#ifndef DEELNEMER_LOCATION_H
#define DEELNEMER_LOCATION_H

#include <QWidget>
#include <QtWebKit>

namespace Ui {
class deelnemer_location;
}

class DeelnemerLocation : public QWidget
{
    Q_OBJECT

public:
    explicit DeelnemerLocation(QString address, QWidget *parent = 0);
    ~DeelnemerLocation();

private:
    Ui::deelnemer_location *ui;
};

#endif // DEELNEMER_LOCATION_H
