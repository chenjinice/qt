#include "delegate.h"
#include <QDebug>
#include <QTextEdit>
#include <QPainter>
#include <QTableWidgetItem>

Delegate::Delegate(QTableWidget *parent)
            : QStyledItemDelegate(parent)
{
    m_table = parent;
}

Delegate::~Delegate()
{

}

void Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid()){
        QString text = index.model()->data(index,Qt::DisplayRole).toString();
        QTableWidgetItem *item = m_table->item(index.row(),index.column());
        QTextOption op((Qt::Alignment)item->textAlignment());
        painter->drawText(option.rect,text,op);
    }else {
        QStyledItemDelegate::paint(painter,option,index);
    }
}

QWidget *Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid()){
        QTextEdit *textedit = new QTextEdit(parent);
        return textedit;
    }else {
         return QStyledItemDelegate::createEditor(parent,option,index);
    }
}

void Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.isValid()){
        QTextEdit *edit = qobject_cast<QTextEdit *>(editor);
        QString text = index.model()->data(index,Qt::DisplayRole).toString();
        edit->setText(text);
    }else{
        QStyledItemDelegate::setEditorData(editor,index);
    }
}

void Delegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.isValid()){
        QTextEdit *edit = qobject_cast<QTextEdit *>(editor);
        QString text = edit->toPlainText();
        model->setData(index,text);
    }else{
        QStyledItemDelegate::setEditorData(editor,index);
    }
}


