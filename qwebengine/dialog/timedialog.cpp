#include "timedialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDateTimeEdit>
#include <QLabel>
#include <QSpacerItem>
#include <QDebug>
#include <QStringList>

timeDialog::timeDialog()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setWindowTitle(tr("选择时间"));
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    mainLayout->setMargin(50);
    this->setLayout(mainLayout);

    m_format = "yyyy-MM-dd HH:mm:ss";
    QHBoxLayout *startLayout = new QHBoxLayout();
    QLabel *startLabel = new QLabel("开始时间 : ");
    m_startTimeEdit = new QDateTimeEdit();
    m_startTimeEdit->setDisplayFormat(m_format);
    m_startTimeEdit->setDateTime(QDateTime::currentDateTime());
    startLayout->addWidget(startLabel);
    startLayout->addWidget(m_startTimeEdit);

    QHBoxLayout *endLayout = new QHBoxLayout();
    QLabel *endLabel = new QLabel("结束时间 : ");
    m_endTimeEdit = new QDateTimeEdit();
    m_endTimeEdit->setDisplayFormat(m_format);
    m_endTimeEdit->setDateTime(QDateTime::currentDateTime());
    endLayout->addWidget(endLabel);
    endLayout->addWidget(m_endTimeEdit);

    QHBoxLayout *hLayout = new QHBoxLayout();
    QPushButton *ok = new QPushButton(tr("OK"),this);
    QPushButton *cancel = new QPushButton(tr("Cancel"),this);
    hLayout->addWidget(ok);
    hLayout->addWidget(cancel);

    mainLayout->addLayout(startLayout);
    mainLayout->addLayout(endLayout);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(hLayout);

    connect(ok,&QPushButton::clicked,this,&timeDialog::ok_clicked);
    connect(cancel,&QPushButton::clicked,this,&timeDialog::cancel_clicked);
}

timeDialog::~timeDialog()
{

}

QStringList timeDialog::getTime()
{
    QStringList list;
    if(this->exec() == Accepted){
        list.append(m_startTimeEdit->dateTime().toString(m_format));
        list.append(m_endTimeEdit->dateTime().toString(m_format));
    }
    return list;
}

void timeDialog::ok_clicked()
{
    this->accept();
}

void timeDialog::cancel_clicked()
{
    this->reject();
}
