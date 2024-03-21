#include <QPushButton>
#include <QBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QSettings>
#include <QLabel>
#include <QDebug>
#include <QMenuBar>
#include <QAction>
#include <QMovie>
#include <QDesktopWidget>
#include <QApplication>
#include "mainwindow.h"
#include "setting.h"
#include "mytable.h"
#include "mydialog.h"
#include "common.h"
#include "udpthread.h"
#include "sethidedialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_udpThread = UdpThread::ins();
    m_table = new MyTable;
    connect(m_udpThread,&UdpThread::bindState,this,&MainWindow::udpState);
    connect(m_udpThread,&UdpThread::getIp,m_table,&MyTable::addIp);

    this->createMenu();
    this->createCentralWidget();
    this->createCombo();
    this->createButton();
    m_layout->addWidget(m_table);
    this->setMinimumSize(1000,500);
    this->setWindowTitle("测试软件 - V2020.10.20");
}

MainWindow::~MainWindow()
{
    delete m_udpThread;
    delete m_table;
}

void MainWindow::createMenu()
{
    QMenuBar *menu_bar = new QMenuBar;
    QMenu *setting_menu = new QMenu("设置");
    QAction *show_a = new QAction("隐藏列",this);
    setting_menu->addAction(show_a);
    menu_bar->addMenu(setting_menu);

    QMenu *  about_menu = new QMenu("关于");
    QAction *aboutqt_a = new QAction("Qt版本",this);
    QAction *aboutme_a = new QAction("码农",this);

    about_menu->addAction(aboutqt_a);
    about_menu->addAction(aboutme_a);
    menu_bar->addMenu(about_menu);

    this->setMenuBar(menu_bar);

    SetHideDialog *dialog = SetHideDialog::ins();
    connect(show_a,&QAction::triggered,dialog,&SetHideDialog::show);
    connect(aboutqt_a,&QAction::triggered,qApp,&QApplication::aboutQt);
    connect(aboutme_a,&QAction::triggered,this,&MainWindow::aboutMe);
    connect(dialog,&SetHideDialog::hideSignal,m_table,&MyTable::hideItem);
}

void MainWindow::createCentralWidget()
{
    QWidget *widget = new QWidget;
    m_layout = new QVBoxLayout;
    widget->setLayout(m_layout);
    this->setCentralWidget(widget);
}

void MainWindow::createCombo()
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setAlignment(Qt::AlignLeft);
    QStringList l = getSelfIp();
    QLabel *label_card     = new QLabel(tr("Udp ："));
    m_labelState           = new QLabel;
    m_labelState->setFixedSize(15,15);
    m_labelState->setScaledContents(true);
    QComboBox *combo = new QComboBox;
    for(int i=0;i<l.count();i++){
        combo->addItem(l.at(i));
        if(l.at(i) == Setting::ins()->getCard()){
            combo->setCurrentText(l.at(i));
        }
    }
    hbox->addWidget(label_card);
    hbox->addWidget(combo);
    hbox->addWidget(m_labelState);
    if(!combo->currentText().isEmpty()){
        QString ip = combo->currentText();
        m_udpThread->startThread(ip);
    }
    m_layout->addLayout(hbox);
    connect(combo,&QComboBox::currentTextChanged,m_udpThread,&UdpThread::setIp);
}

void MainWindow::createButton()
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setAlignment(Qt::AlignLeft);
    QLabel *label_other = new QLabel("其他 ：");
    QCheckBox *sort_box = new QCheckBox("按ip排序");
    QPushButton *clear = new QPushButton("清表格");
    QPushButton *add = new QPushButton("添ip");
    hbox->addWidget(label_other);
//    hbox->addWidget(check_box);
    hbox->addWidget(sort_box);
    hbox->addWidget(clear);
    hbox->addWidget(add);
    m_layout->addLayout(hbox);
    sort_box->setChecked(Setting::ins()->getAutoSort());

    MyDialog *dialog = MyDialog::ins();
    connect(add,&QPushButton::clicked,dialog,&MyDialog::show);
    connect(dialog,&MyDialog::addIp,m_table,&MyTable::addIp);
    connect(clear,&QPushButton::clicked,this,&MainWindow::clearClicked);
    connect(sort_box,&QCheckBox::stateChanged,this,&MainWindow::sortBoxChanged);
}

void MainWindow::aboutMe()
{
    static QDialog *dialog = NULL;
    static QDesktopWidget *deskdop = QApplication::desktop();
    if(dialog == NULL){
        dialog = new QDialog;
        QVBoxLayout *layout = new QVBoxLayout;
        QLabel *picture = new QLabel;
        QMovie *gif = new QMovie(":/icon/image/me.gif");
        picture->setMovie(gif);
        QPushButton *button = new QPushButton("关闭");
        gif->start();

        layout->addWidget(picture);
        layout->addWidget(button);
        dialog->setLayout(layout);
        dialog->setWindowTitle("深渊凝视");
        dialog->move((deskdop->width()-dialog->width())/2,(deskdop->height()-dialog->height())/2);
        dialog->setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint|Qt::MSWindowsFixedSizeDialogHint);

        connect(button,&QPushButton::clicked,dialog,&QDialog::close);
    }
    dialog->show();
}

void MainWindow::udpState(bool flag)
{
    static QPixmap yes(":/icon/image/connect_yes.png");
    static QPixmap no(":/icon/image/connect_no.png");
    if(flag)m_labelState->setPixmap(yes);
    else m_labelState->setPixmap(no);
}

void MainWindow::clearClicked()
{
    m_table->clearResult();
}

void MainWindow::sortBoxChanged(int value)
{
    bool flag = false;
    if(value > 0){
        flag = true;
        m_table->sortByIp();
    }
    Setting::ins()->setAutoSort(flag);
}

