#include "giswindow.h"
#include "mytree.h"
#include "mylist.h"
#include "mapconfig.h"
#include <QGridLayout>
#include <QListWidget>

#include <qgsproviderregistry.h>
#include <qgsmaplayerregistry.h>
#include <qgsmapcanvas.h>
#include <qgsvectorlayer.h>

#include <QModelIndexList>

GisWindow::GisWindow(QWidget *parent) : QWidget(parent)
{
    QGridLayout *gridLayout = new QGridLayout;
    this->setLayout(gridLayout);

    //file tree
    m_treeView = new MyTree;
    m_treeView->setMaximumWidth(400);
    gridLayout->addWidget(m_treeView,0,0,1,1);
    connect(m_treeView,SIGNAL(addAction_clicked()),this,SLOT(add_slot()));

    // mapcanvas
    QgsProviderRegistry::instance("/home/chen/plugins/");
    m_mapCanvas = new QgsMapCanvas(0,0);
    gridLayout->addWidget(m_mapCanvas,0,1,2,1);

    // list
    m_listWidget = new MyList;
    m_listWidget->setMaximumWidth(400);
    m_listWidget->setDragEnabled(true);
    m_listWidget->setAcceptDrops(true);
    m_listWidget->setDropIndicatorShown(true);
    m_listWidget->setDragDropMode(QAbstractItemView::DragDrop);
    gridLayout->addWidget(m_listWidget,1,0,1,1);
    connect(m_listWidget,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(test(QListWidgetItem*)));
    connect(m_listWidget,SIGNAL(indexsChanged()),this,SLOT(indexChanged_slot()));
}

GisWindow::~GisWindow()
{

}

void GisWindow::addLayer(QString path)
{
    if(m_openedFile.indexOf(path)  != -1) return;
    QString str = QFileInfo(path).completeBaseName();
    QgsVectorLayer *mapLayer = new QgsVectorLayer(path,str,"ogr");
    if(mapLayer->isValid())
    {
        m_mapLaySet.append(QgsMapCanvasLayer(mapLayer,true));
        QgsMapLayerRegistry::instance()->addMapLayer(mapLayer);
        m_mapCanvas->setExtent(mapLayer->extent());
        m_mapCanvas->setLayerSet(m_mapLaySet);
        m_openedFile.append(path);
        m_mapConfigList.append(MapConfig(path));
        this->listAddItem(path);
    }
}

void GisWindow::listAddItem(QString path)
{
    QListWidgetItem *item = new QListWidgetItem(QFileInfo(path).completeBaseName());
    item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled);
    item->setCheckState(Qt::Checked);
    m_listWidget->addItem(item);

//    for(int i=0;i<m_mapConfigList.length();i++)
//    {
//        MapConfig *config = &m_mapConfigList[i];
//        qDebug() << config->baseName() << " : " << config->path() << " : " << config->isEnabled();
//    }

}

void GisWindow::add_slot()
{
    QStringList *list = m_treeView->selectedItems();
    for(int i=0;i<list->length();i++)
    {
        qDebug() << list->at(i);
        this->addLayer(list->at(i));
    }
}

void GisWindow::test(QListWidgetItem *item)
{
    qDebug() << item->checkState() << " : "<< m_listWidget->row(item);
}

void GisWindow::indexChanged_slot()
{
    qDebug() << "index changed ";
}

















