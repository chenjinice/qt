#include "mylist.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QListWidgetItem>

MyList::MyList(QWidget *parent) : QListWidget(parent)
{

}

MyList::~MyList()
{

}

void MyList::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->source() == this)
    {
//        qDebug() <<  this->row(this->currentItem());
        event->accept();
    }else
    {
        event->ignore();
    }
}

void MyList::dragMoveEvent(QDragMoveEvent *event)
{
//    qDebug() << " move ";
    QListWidgetItem *item = this->itemAt(event->pos());
    if(!item)
    {
        return;
    }
    item->setSelected(true);
    this->scrollToItem(item);
//    QFont font;
//    int length = this->count();
//    for(int i=0;i<length;i++){
//        if(this->item(i) == item)
//        {
//            qDebug() << this->item(i)->text();
//            font.setUnderline(true);
//            this->item(i)->setFont(font);
//        }
//        else
//        {
//            qDebug() << this->item(i);
//            font.setUnderline(false);
//            this->item(i)->setFont(font);
//        }
//    }
}

void MyList::dropEvent(QDropEvent *event)
{
//    qDebug() << "drop event : " << event->pos();
//    QListWidgetItem *select_Item = this->selectedItems()[0];
    QListWidgetItem *item = this->currentItem();
    QListWidgetItem *select_Item = this->selectedItems()[0];
    if(item == select_Item)
    {
        event->ignore();
        return;
    }

    int index = this->row(select_Item)+1;
    if(index < 0 || index > this->count() || index == this->row(item))
    {
        event->ignore();
        return;
    }else {
        QListWidgetItem *itemClone = item->clone();
        this->insertItem(index,itemClone);
        delete item;
        itemClone->setSelected(true);
        this->scrollToItem(itemClone);
        emit  indexsChanged();
//        qDebug() << " yes yes";
    }

//    if(this->itemAt(event->pos()))
//    {
//        qDebug() << this->itemAt(event->pos())->text();
//    }
}




