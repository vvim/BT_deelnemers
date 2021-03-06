#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

// taken from https://github.com/vvim/routetool/commit/cc3e484eeb7294ee210e1b03e761d69466d90623

#include <QLineEdit>
#include <QStringList>
#include <QStringListModel>
#include <QString>
#include <QCompleter>
#include <QKeyEvent>
#include <QMouseEvent>

class MyCompleter : public QCompleter
{
    Q_OBJECT

    public:
        inline MyCompleter(const QStringList& words, QObject * parent) :
        QCompleter(parent), m_list(words), m_model()
        {
            setModel(&m_model);
        }

        inline MyCompleter(QObject *parent)
        {
            setParent(parent);
        }

        inline void update(QString word)
        {
            // Do any filtering you like.
            // Here we just include all items that contain word.
            QStringList filtered = m_list.filter(word, caseSensitivity());
            m_model.setStringList(filtered);
            m_word = word;
            complete();
        }

        inline QString word()
        {
            return m_word;
        }

    private:
        QStringList m_list;
        QStringListModel m_model;
        QString m_word;
};

class MyLineEdit : public QLineEdit
{
    Q_OBJECT

    public:
        MyLineEdit(QWidget *parent = 0);
        ~MyLineEdit();

        void setCompleter(MyCompleter *c);
        MyCompleter *completer() const;

    protected:
        void keyPressEvent(QKeyEvent *e);
        void mousePressEvent(QMouseEvent *m);

    private slots:
        void insertCompletion(const QString &completion);

    private:
        MyCompleter *c;
};


#endif // MYLINEEDIT_H
