#include "mylineedit.h"
#include <QDebug>

// taken from https://github.com/vvim/routetool/commit/cc3e484eeb7294ee210e1b03e761d69466d90623

#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

MyLineEdit::MyLineEdit(QWidget *parent)
: QLineEdit(parent), c(0)
{
}

MyLineEdit::~MyLineEdit()
{
}

void MyLineEdit::setCompleter(MyCompleter *completer)
{
    c = completer;
    if (!c)
    {
        vvimDebug() << "something went wrong";
        return;
    }

    c->setWidget(this);
    connect(completer, SIGNAL(activated(const QString&)), this, SLOT(insertCompletion(const QString&)));
}

MyCompleter *MyLineEdit::completer() const
{
    return c;
}

void MyLineEdit::insertCompletion(const QString& completion)
{
    setText(completion);
    selectAll();
}


void MyLineEdit::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible())
    {
        // The following keys are forwarded by the completer to the widget
        switch (e->key())
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
                e->ignore();
                return; // Let the completer do default behavior
        }
    }

    bool isShortcut = (e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E;
    if (!isShortcut)
        QLineEdit::keyPressEvent(e); // Don't send the shortcut (CTRL-E) to the text edit.

    if (!c)
        return;

    bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!isShortcut && !ctrlOrShift && e->modifiers() != Qt::NoModifier)
    {
        c->popup()->hide();
        return;
    }

    c->update(text());
    c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
}

void MyLineEdit::mousePressEvent(QMouseEvent *m)
{
    if (m)
    {
        switch (m->button())
        {
            case Qt::LeftButton:
                // left mousebutton pressed in MyLineEdit, clear it so that the user can type
                vvimDebug() << "left mousebutton pressed in MyLineEdit, clear it so that the user can type";
                this->clear();
                return;
                /*
            case Qt::RightButton:
                qDebug() << "right mousebutton pressed";
                return;
                */
            default:
                m->ignore();
        }
    }
}
