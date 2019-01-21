#include "mainwindow.h"
#include <QToolBar>
#include <QAction>
#include <QVBoxLayout>
#include "gis/giswindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QToolBar *toolBar = this->addToolBar(tr("toolBar"));
    QAction  *confAction = toolBar->addAction(tr("config"));

    mainWidget->setLayout(mainLayout);
    mainLayout->setMargin(0);
    this->setCentralWidget(mainWidget);
    this->setMinimumSize(500,500);

    m_gisWindow = new GisWindow;
    mainLayout->addWidget(m_gisWindow);
    connect(confAction,SIGNAL(triggered(bool)),m_gisWindow,SLOT(show()));
}

MainWindow::~MainWindow()
{

}

