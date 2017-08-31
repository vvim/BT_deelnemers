#include "completerdelegate.h"

#include <QLineEdit>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>


#define vvimDebug()\
    qDebug() << "[" << Q_FUNC_INFO << "]"

CompleterDelegate::CompleterDelegate(QSqlRelationalTableModel *_model_deelnemers, QObject *parent)
:QItemDelegate(parent)
{
    model_deelnemers = _model_deelnemers;

    completer = NULL;
    loadCompleter();

    vvimDebug() << "destructor: delete completer aub";
}


QWidget *CompleterDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    // The createEditor() function is called when the user starts editing an item:
  MyLineEdit *editor = new MyLineEdit(parent);
  //connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor())); -> this is mentioned in http://doc.qt.io/qt-4.8/qt-itemviews-stardelegate-example.html , but do we need it?
  editor->setCompleter(completer);

  return editor;
}

void CompleterDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // get value from the MODEL and bring it to the VIEW

    //zeker lezen !!! http://doc.qt.io/qt-5/qtwidgets-itemviews-stardelegate-example.html
    //            http://doc.qt.io/qt-4.8/qt-itemviews-stardelegate-example.html

    // The setEditorData() function is called when an editor is created to initialize it with data from the model:

  MyLineEdit *lineEdit = static_cast<MyLineEdit*>(editor);
  int deelnemer_id = index.model()->data(index, Qt::EditRole).toInt();
    lineEdit->setText(id_map[deelnemer_id]);
    lineEdit->setCompleter(completer);
}

void CompleterDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // The setModelData() function is called when editing is finished, to commit data from the editor to the model:

    // get value from the VIEW and save it in the MODEL
    MyLineEdit *lineEdit = static_cast<MyLineEdit*>(editor);
  model->setData(index, deelnemers_map[lineEdit->text()], Qt::EditRole);
}

void CompleterDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    // if you do not implement this, everytime you call the delegate, it will be floating in the upper left corner
  editor->setGeometry(option.rect);
}

void CompleterDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // The paint() function is reimplemented from QStyledItemDelegate and is called whenever the view needs to repaint an item

  QStyleOptionViewItemV4 myOption = option;

  int value = index.model()->index(index.row(),index.column()).data(Qt::EditRole).toInt();

  QString deelnemer = id_map[value];
  myOption.text = deelnemer;
  vvimDebug() << value << deelnemer;

  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}


void CompleterDelegate::loadCompleter()
{
    if(completer)
        delete completer;

    deelnemers_map.clear();
    QStringList deelnemers_list;

    int idIdx = model_deelnemers->fieldIndex("id");
    int familieNaamIdx = model_deelnemers->fieldIndex("familienaam");
    int naamIdx = model_deelnemers->fieldIndex("naam");
    vvimDebug() << "[CAVEAT]"
                << "we expect the combination [naam] [familienaam] to be unique, but can we guarantee that?"
                << "else we could mix the address of telephonenumber in the mix?"
                << "currently I add there ID-number to make every entry unique";

    for ( int i = 0 ; i < model_deelnemers->rowCount() ; ++i )
    {
        QString dlnmr;
        if( model_deelnemers->index( i, familieNaamIdx ).data().isNull())
        {
            dlnmr = model_deelnemers->index( i, naamIdx ).data().toString();
        }
        else
        {
            dlnmr = model_deelnemers->index( i, naamIdx ).data().toString();
            dlnmr.append(" ");
            dlnmr.append(model_deelnemers->index( i, familieNaamIdx ).data().toString());
        }
        dlnmr = dlnmr.simplified();
        while(dlnmr.endsWith(" -"))
        {
            dlnmr.chop(2);
        }
        deelnemers_list << dlnmr;

        int id_deelnemer = model_deelnemers->index( i, idIdx ).data().toInt();
        deelnemers_map[dlnmr] = id_deelnemer;
        id_map[id_deelnemer] = dlnmr;
    }

    deelnemers_list.sort();
    completer = new MyCompleter(deelnemers_list, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    vvimDebug() << "done, completer (re)loaded." << deelnemers_map.count() << id_map.count() << "rows";
}
