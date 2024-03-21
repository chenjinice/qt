#include <QBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QCheckBox>
#include "sethidedialog.h"
#include "common.h"
#include "setting.h"

SetHideDialog * SetHideDialog::m_instance = nullptr;


SetHideDialog *SetHideDialog::ins()
{
    if(m_instance == nullptr){
        m_instance = new SetHideDialog;
    }
    return m_instance;
}

SetHideDialog::SetHideDialog()
{
    QVBoxLayout *    layout        = new QVBoxLayout;
    layout->setAlignment(Qt::AlignCenter);
    QLabel *         label         = new QLabel("隐藏的列打勾 : ");
    layout->addWidget(label);

    QStringList l = getTestItemsUi();
    l.removeAll(CI_IP);
    for(int i=0;i<l.length();i++){
        QString      item          = l.at(i);
        bool         hide         = Setting::ins()->getHide(item);
        QCheckBox *  checkbox      = new QCheckBox;
        QHBoxLayout *h             = new QHBoxLayout;
        QLabel *     num_label     = new QLabel(QString("%1.").arg(i+1,2,10,QChar(' ')));
        checkbox->setText(item);
        if(hide)checkbox->setChecked(true);
        h->setAlignment(Qt::AlignLeft);
        h->addWidget(num_label);
        h->addWidget(checkbox);
        layout->addLayout(h);
        m_list.push_back(checkbox);
        connect(checkbox,&QCheckBox::stateChanged,this,&SetHideDialog::checkboxChanged);
    }
    QPushButton *button_all = new QPushButton("全选");
    QPushButton *button_ok = new QPushButton("关闭");
    button_ok->setDefault(true);
    layout->addWidget(button_all);
    layout->addWidget(button_ok);
    this->setLayout(layout);
    this->setWindowTitle("隐藏列");
    Qt::WindowFlags flags = Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint|Qt::MSWindowsFixedSizeDialogHint;
    this->setWindowFlags(flags);
    this->setMinimumWidth(200);
    connect(button_all,&QPushButton::clicked,this,&SetHideDialog::selectAll);
    connect(button_ok,&QPushButton::clicked,this,&SetHideDialog::close);
}

void SetHideDialog::checkboxChanged(int value)
{
    QCheckBox * s     = qobject_cast<QCheckBox *>(sender());
    QString     item  = s->text();
    bool        flag  = false;

    if(value == Qt::Checked) flag = true;
    Setting::ins()->setHide(item,flag);
    emit hideSignal(item,flag);
}

void SetHideDialog::selectAll()
{
    static bool s_bool = false;
    for(int i=0;i<m_list.length();i++){
        QCheckBox * s     = qobject_cast<QCheckBox *>(m_list.at(i));
        s->setChecked(s_bool);
    }
    if(s_bool)s_bool = false;
    else s_bool = true;
}

SetHideDialog::~SetHideDialog()
{

}
