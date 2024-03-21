#include <QBoxLayout>
#include <QPushButton>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QFont>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include "setting.h"
#include "mydialog.h"
#include "udpthread.h"


MyDialog *MyDialog::m_instance = nullptr;


MyDialog *MyDialog::ins()
{
    if(m_instance == nullptr){
        m_instance = new MyDialog;
    }
    return m_instance;
}

MyDialog::MyDialog()
{
    QFont font( "Microsoft YaHei",10,75);
    QRegExp rx("^(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])");
    QRegExpValidator *  reg = new QRegExpValidator(rx, this);
    int   width             = 50;
    QString dot             = ".";

    this->setWindowTitle("添加IP");
    this->setFixedSize(300,200);
    QVBoxLayout *v          = new QVBoxLayout;
    QHBoxLayout *h1         = new QHBoxLayout;
    QHBoxLayout *h_from     = new QHBoxLayout;
    QHBoxLayout *h_to       = new QHBoxLayout;
    QHBoxLayout *h2         = new QHBoxLayout;

    QLabel *label_to        = new QLabel(tr("↓"));
    QLabel *   d1           = new QLabel(dot);d1->setFont(font);
    QLabel *   d2           = new QLabel(dot);d2->setFont(font);
    QLabel *   d3           = new QLabel(dot);d3->setFont(font);
    QLabel *   d5           = new QLabel(dot);d5->setFont(font);
    QLabel *   d6           = new QLabel(dot);d6->setFont(font);
    QLabel *   d7           = new QLabel(dot);d7->setFont(font);
    m_ip1                   = new QLineEdit(this->getSavedValue(CS_IP1));m_ip1->setFixedWidth(width);m_ip1->setValidator(reg);
    m_ip2                   = new QLineEdit(this->getSavedValue(CS_IP2));m_ip2->setFixedWidth(width);m_ip2->setValidator(reg);
    m_ip3                   = new QLineEdit(this->getSavedValue(CS_IP3));m_ip3->setFixedWidth(width);m_ip3->setValidator(reg);
    m_ip4                   = new QLineEdit(this->getSavedValue(CS_IP4));m_ip4->setFixedWidth(width);m_ip4->setValidator(reg);
    m_to1                   = new QLabel(this->getSavedValue(CS_IP1));   m_to1->setFixedWidth(width);
    m_to2                   = new QLabel(this->getSavedValue(CS_IP2));   m_to2->setFixedWidth(width);
    m_to3                   = new QLabel(this->getSavedValue(CS_IP3));   m_to3->setFixedWidth(width);
    m_to4                   = new QLineEdit(this->getSavedValue(CS_TO4));m_to4->setFixedWidth(width);m_to4->setValidator(reg);

    QPushButton *bt_yes = new QPushButton(tr("确定"));
    QPushButton *bt_no  = new QPushButton(tr("取消"));

    h1->setAlignment(Qt::AlignCenter);
    h1->addWidget(label_to);
    h_from->addWidget(m_ip1);h_from->addWidget(d1);h_from->addWidget(m_ip2);h_from->addWidget(d2);
    h_from->addWidget(m_ip3);h_from->addWidget(d3);h_from->addWidget(m_ip4);
    h_to->addWidget(m_to1);h_to->addWidget(d5);h_to->addWidget(m_to2);h_to->addWidget(d6);
    h_to->addWidget(m_to3);h_to->addWidget(d7);h_to->addWidget(m_to4);
    h2->addWidget(bt_yes);h2->addWidget(bt_no);

    v->setMargin(30);
    v->addLayout(h_from);v->addLayout(h1);v->addLayout(h_to);
    v->addSpacing(20);v->addLayout(h2);
    this->setLayout(v);
    Qt::WindowFlags flags = Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint;
    this->setWindowFlags(flags);

    connect(bt_yes,&QPushButton::clicked,this,&MyDialog::yesClicked);
    connect(bt_no,&QPushButton::clicked,this,&MyDialog::noClicked);
    connect(m_ip1,&QLineEdit::textChanged,this,&MyDialog::changedSlot);
    connect(m_ip2,&QLineEdit::textChanged,this,&MyDialog::changedSlot);
    connect(m_ip3,&QLineEdit::textChanged,this,&MyDialog::changedSlot);
    connect(m_ip4,&QLineEdit::textChanged,this,&MyDialog::changedSlot);
    connect(m_to4,&QLineEdit::textChanged,this,&MyDialog::changedSlot);
}

MyDialog::~MyDialog()
{
}

void MyDialog::yesClicked()
{
    uint8_t count = 1;
    uint8_t start = m_ip4->text().toUInt();
    uint8_t end   = m_to4->text().toUInt();
    int minus = end - start;
    if( minus > 0)count += minus;
    for(int i=0;i<count;i++){
        QString tail = QString::number(start+i);
        QString ip = QString("%1.%2.%3.%4").arg(m_ip1->text(),m_ip2->text(),m_ip3->text(),tail);
        UdpThread::ins()->add(ip);
    }
    this->hide();
}

void MyDialog::noClicked()
{
    this->hide();
}

QString MyDialog::getSavedValue(const QString &key)
{
    uint8_t ret = Setting::ins()->value(key).toUInt();
    QString str = QString::number(ret);
    return  str;
}

void MyDialog::changedSlot(const QString &str)
{
    QLineEdit *s = qobject_cast<QLineEdit *>(sender());
    QString key;
    if(s == m_ip1){
        key = CS_IP1;
        m_to1->setText(str);
    }else if(s == m_ip2){
        key = CS_IP2;
        m_to2->setText(str);
    }else if(s == m_ip3){
        key = CS_IP3;
        m_to3->setText(str);
    }else if(s == m_ip4){
        key = CS_IP4;
    }else if(s == m_to4){
        key = CS_TO4;
    }
    if(key.length()>0){
        Setting::ins()->setValue(key,str);
    }
}

