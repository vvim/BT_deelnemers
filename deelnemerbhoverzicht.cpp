#include "deelnemerbhoverzicht.h"
#include "ui_deelnemerbhoverzicht.h"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"


DeelnemerBHOverzicht::DeelnemerBHOverzicht(SDeelnemerMarker _deelnemer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeelnemerBHOverzicht)
{
    settings = new QSettings("settings.ini", QSettings::IniFormat);

    urloverview = settings->value("bh/urloverview").toString();

    urlinput = settings->value("bh/urlinput").toString();
    QString minguser = settings->value("minguser").toString();
    urlinput.append(QString("%1.php").arg(minguser.toLower()));

    ui->setupUi(this);

    deelnemer_id = _deelnemer.id;
    overzicht_max_lines = 10;

    ui->spinbox_max_lines->setValue(overzicht_max_lines);
    showOverzicht();

    ui->radioButton_toon_overzicht->setChecked(true);
}

DeelnemerBHOverzicht::~DeelnemerBHOverzicht()
{
    delete ui;
}

void DeelnemerBHOverzicht::on_pushButton_set_max_lines_clicked()
{
    overzicht_max_lines = ui->spinbox_max_lines->value();
    vvimDebug() << "max lijntjes is nu" << overzicht_max_lines;
    showOverzicht();
}

void DeelnemerBHOverzicht::showOverzicht()
{
    ui->webView->setStyleSheet("background-color: white");
    QString url = QString("%1?q=%2").arg(urloverview).arg(deelnemer_id);
    if(overzicht_max_lines > 0)
        url = QString("%1&limit=%2").arg(url).arg(overzicht_max_lines);
    vvimDebug() << "max lines:" << overzicht_max_lines;
    vvimDebug() << "url:" << url;
    ui->webView->load(QUrl(url));
}

void DeelnemerBHOverzicht::print(QPrinter* printer)
{
    ui->webView->print(printer);
}

void DeelnemerBHOverzicht::on_pushButton_print_clicked()
{
    if (!m_printer) m_printer.reset(new QPrinter);
    QScopedPointer<QPrintDialog> dialog(new QPrintDialog(m_printer.data(), this));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog.data(), SIGNAL(accepted(QPrinter*)), SLOT(print(QPrinter*)));
    dialog->show();
    dialog.take(); // The dialog will self-delete}
}

void DeelnemerBHOverzicht::on_radioButton_toon_overzicht_toggled(bool checked)
{
    ui->spinbox_max_lines->setVisible(checked);
    ui->label_max_lines->setVisible(checked);
    ui->pushButton_print->setVisible(checked);
    ui->pushButton_set_max_lines->setVisible(checked);

    if(checked)
    {
        showOverzicht();
    }
    else
    {
        ui->webView->setStyleSheet("background-color: none");
        ui->webView->load(QUrl(urlinput));
    }
}
