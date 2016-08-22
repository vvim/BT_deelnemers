#ifndef DEELNEMER_LOCATION_H
#define DEELNEMER_LOCATION_H

#include <QWidget>
#include <QWebPage>
#include <QSettings>
#include <QSqlRelationalTableModel>
#include "sdeelnemermarker.h"

namespace Ui {
class deelnemer_location;
}

class myWebPage : public QWebPage
{
    virtual QString userAgentForUrl(const QUrl& url) const {
        // return "Chrome/1.0"; // see https://wiki.qt.io/How_to_set_user_agent_in_Qt_application
        return "Mozilla/5.0 (X11; Linux i686; rv:40.0) Gecko/20100101 Firefox/40.0";
    }
};

class DeelnemerLocation : public QWidget
{
    Q_OBJECT

public:
    explicit DeelnemerLocation(SDeelnemerMarker *_deelnemer, QSqlRelationalTableModel *_model_deelnemers, int _zoom = 15, QWidget *parent = 0);
    ~DeelnemerLocation();

private slots:
    void on_pushButton_showAllDeelnemers_clicked();

    void on_pushButton_TestButton_clicked();

private:
    Ui::deelnemer_location *ui;
    QSettings *settings;
    SDeelnemerMarker *deelnemerMarker;
    QSqlRelationalTableModel *model_deelnemers;
    int zoom;
};

#endif // DEELNEMER_LOCATION_H
