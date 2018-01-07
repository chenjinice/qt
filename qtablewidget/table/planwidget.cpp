#include "planwidget.h"
#include "mytable.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QDebug>
#include <QLabel>
#include <QFileDialog>
#include <QLineEdit>
#include <QCloseEvent>
#include <QTime>

PlanWidget::PlanWidget(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout)
{
    this->setLayout(m_layout);
    this->createInfoWidgets();
    m_table = new MyTable;
    m_layout->addWidget(m_table);

    QHBoxLayout *hlayout = new QHBoxLayout;
    QPushButton *save = new QPushButton("save",this);
    QPushButton *open = new QPushButton("open",this);
    QPushButton *debug = new QPushButton("debug",this);
    connect(save,&QPushButton::clicked,this,&PlanWidget::save);
    connect(open,&QPushButton::clicked,this,&PlanWidget::open);
    connect(debug,&QPushButton::clicked,this,&PlanWidget::printallDate);
    hlayout->addWidget(save);
    hlayout->addWidget(open);
    hlayout->addWidget(debug);
    m_layout->addLayout(hlayout);
//    this->resize(800,500);
}

PlanWidget::~PlanWidget()
{

}

QString PlanWidget::title()
{
    return m_title->text();
}

QString PlanWidget::examine()
{
    return m_exam->text();
}

QString PlanWidget::leader()
{
    return m_leader->text();
}

QString PlanWidget::teacher()
{
    return m_teacher->text();
}

QString PlanWidget::editTime()
{
    return m_time->text();
}

QString PlanWidget::planData()
{
    return m_table->getTableData();
}

void PlanWidget::setTitle(const QString &str)
{
    m_title->setText(str);
}

void PlanWidget::setExamine(const QString &str)
{
    m_exam->setText(str);
}

void PlanWidget::setLeader(const QString &str)
{
    m_leader->setText(str);
}

void PlanWidget::setTeacher(const QString &str)
{
    m_teacher->setText(str);
}

void PlanWidget::setEditTime(const QString &str)
{
    m_time->setText(str);
}

void PlanWidget::setPlanData(QString &str)
{
    m_table->setTableData(str);
}

void PlanWidget::printValidData()
{
    int row = m_table->rowCount();
    int col = m_table->columnCount();
    for(int i=0;i<row;i++){
        QString str;
        for(int m=0;m<col;m++){
            QTableWidgetItem *item = m_table->item(i,m);
            if(item && !item->text().isEmpty()) str += item->text()+",";
        }
        if(!str.isEmpty())qDebug()<< str;
    }
}

void PlanWidget::printallDate()
{
    int start = QDateTime::currentMSecsSinceEpoch();
    int row = m_table->rowCount();
    int col = m_table->columnCount();
    for(int i=0;i<row;i++){
        QString str;
        QString str2;
        for(int m=0;m<col;m++){
            QTableWidgetItem *item = m_table->item(i,m);
            if(item) {
                str += QString("%1,%2:%3|").arg(i).arg(m).arg(item->text());
                str2 += QString("%1,%2|").arg(m_table->rowSpan(i,m)).arg(m_table->columnSpan(i,m));
            }
        }
        qDebug()<< str << "======" << str2;
    }
    int end = QDateTime::currentMSecsSinceEpoch();
    double last = (end-start)*1.0/1000.0;
    qDebug() << last;
}

void PlanWidget::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void PlanWidget::createInfoWidgets()
{
    QHBoxLayout *titleLayout = new QHBoxLayout;
    QLabel *title = new QLabel(tr("标题 : "));
    m_title = new QLineEdit;
    m_title->setText("工作安排表");
    titleLayout->addWidget(title);
    titleLayout->addWidget(m_title);

    QHBoxLayout *leaderLayout = new QHBoxLayout;
    QLabel *exam = new QLabel(tr("审批 : "));
    m_exam = new QLineEdit;
    QLabel *leader = new QLabel(tr("副队长 : "));
    m_leader = new QLineEdit;
    QLabel *teacher = new QLabel(tr("代理教导员 : "));
    m_teacher = new QLineEdit;
    leaderLayout->addWidget(exam);
    leaderLayout->addWidget(m_exam);
    leaderLayout->addWidget(leader);
    leaderLayout->addWidget(m_leader);
    leaderLayout->addWidget(teacher);
    leaderLayout->addWidget(m_teacher);

    QHBoxLayout *timeLayout = new QHBoxLayout;
    QLabel *time = new QLabel(tr("修改时间 : "));
    m_time = new QLabel(tr("2017"));
    timeLayout->addWidget(time);
    timeLayout->addWidget(m_time);

    m_layout->addLayout(titleLayout);
    m_layout->addLayout(leaderLayout);
    m_layout->addLayout(timeLayout);
}

void PlanWidget::save()
{
    QString path = QFileDialog::getSaveFileName(this,tr("保存文件"),"./a.plan","*.plan");
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
//        qDebug() << tr("打开文件失败 : ") << path;
        return;
    }
    QTextStream out(&file);
    out << m_table->getTableData();
}

void PlanWidget::open()
{
    QString path = QFileDialog::getOpenFileName(this,"get file","./","*.plan");
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
//        qDebug() << tr("打开文件失败 : ") << path;
        return;
    }
    QTextStream in(&file);
    QString data = in.readAll();
    m_table->setTableData(data);
}

