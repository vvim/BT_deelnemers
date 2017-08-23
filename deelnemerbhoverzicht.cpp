#include "deelnemerbhoverzicht.h"
#include "ui_deelnemerbhoverzicht.h"

#include "../bluetiger9/SmtpClient-for-Qt-1.1/src/SmtpMime"

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"


DeelnemerBHOverzicht::DeelnemerBHOverzicht(SDeelnemerMarker _deelnemer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeelnemerBHOverzicht)
{
    ui->setupUi(this);
    ui->webView->setStyleSheet("background-color: white");

    deelnemer_id = _deelnemer.id;
    overzicht_max_lines = 10;

    ui->spinbox_max_lines->setValue(overzicht_max_lines);
    showOverzicht();
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
    QString url = QString("http://www.buurtijd.be/bh/gettransactions.php?q=%1").arg(deelnemer_id);
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

void DeelnemerBHOverzicht::on_pushButton_email_clicked()
{
    // This is a first demo application of the SmtpClient for Qt project


    // First we need to create an SmtpClient object
    // We will use the Gmail's smtp server (smtp.gmail.com, port 465, ssl)

    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    // We need to set the username (your email address) and password
    // for smtp authentification.

    smtp.setUser("email @gmail.com");
    smtp.setPassword("super d00per secret password!");

    // Now we create a MimeMessage object. This is the email.

    MimeMessage message;

    EmailAddress sender("smtp client @host.com", "Qt Iverse");
    message.setSender(&sender);

    EmailAddress to("throw away email", "Throwzies");
    message.addRecipient(&to);

    message.setSubject("Mail from Qt Program");

    // Now add some text to the email.
    // First we create a MimeText object.

    MimeText text;

    //text.setText("Hi,\nThis is a simple email message.\n");
    text.setText(ui->webView->page()->mainFrame()->toHtml());

    // Now add it to the mail

    message.addPart(&text);

    // Now we can send the mail

    if (!smtp.connectToHost()) {
        vvimDebug() << "Failed to connect to host!" << endl;
        return;
    }

    if (!smtp.login()) {
        vvimDebug() << "Failed to login!" << endl;
        return;
    }

    if (!smtp.sendMail(message)) {
        vvimDebug() << "Failed to send mail!" << endl;
        return;
    }

    vvimDebug() << "mail is sent";

    smtp.quit();
}
