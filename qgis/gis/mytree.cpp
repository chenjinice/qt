#include "mytree.h"
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QStringList>
#include <QFileSystemModel>
#include <QModelIndexList>

MyTree::MyTree(QWidget *parent):QTreeView(parent)
{
    QStringList filters;
    m_model = new QFileSystemModel;

    filters.append("*.shp");
    m_model->setRootPath(QDir::homePath());
    m_model->setNameFilters(filters);
    m_model->setNameFilterDisables(false);

    this->setModel(m_model);
    this->setRootIndex(m_model->index("/home/chen/Desktop/chen"));
    this->setColumnHidden(1,true);
    this->setColumnHidden(2,true);
    this->setColumnHidden(3,true);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(treeMenu(QPoint)));
}

MyTree::~MyTree()
{
    delete m_model;
}

QStringList *MyTree::selectedItems()
{
    return &m_list;
}

void MyTree::treeMenu(QPoint p)
{
    m_list.clear();
    QModelIndexList list = this->selectedIndexes();
    if(list.length() <= 0) return;
    for(int i=0;i<list.length();i++)
    {
        if(m_model->fileInfo(list[i]).isDir())
        {
            m_list.clear();
            return;
        }else
        {
            m_list.append(m_model->filePath(list[i]));
        }
    }

    QMenu *menu = new QMenu;
    QAction *action = new QAction(tr("add"),0);
    menu->addAction(action);
    connect(action,SIGNAL(triggered(bool)),this,SLOT(addFiles()));
    menu->exec(QCursor::pos());

    disconnect(action);
    delete action;
    delete menu;
}

void MyTree::addFiles()
{
    emit addAction_clicked();
}




