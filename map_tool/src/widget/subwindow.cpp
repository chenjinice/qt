#include "subwindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QLineEdit>
#include <QDesktopWidget>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include "msg.h"


SubWindow::SubWindow(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    m_webview           = new WebView(HTML_INDEX);
    layout->setMargin(0);
    layout->addWidget(m_webview);
    this->setLayout(layout);
}

SubWindow::~SubWindow()
{
    delete m_webview;
}

void SubWindow::addClicked()
{
    if(!this->isActiveWindow())return;
    emit m_webview->obj()->addMarker();
}

void SubWindow::addAtClicked()
{
    if(!this->isActiveWindow())return;
    static QDialog *   dialog  = nullptr;
    static QLineEdit *lat_edit = new QLineEdit;
    static QLineEdit *lng_edit = new QLineEdit;
    static QDesktopWidget *deskdop = QApplication::desktop();

    if(dialog == nullptr){
        dialog = new QDialog;
        QGridLayout *layout = new QGridLayout;
        QLabel *lat_label = new QLabel("纬度");
        QLabel *lng_label = new QLabel("经度");
        QLabel *ex_label  = new QLabel("例如 : 112.1234567 , 22.7654321");
        QPushButton *ok_button = new QPushButton("确定");
        QPushButton *cancel_button = new QPushButton("取消");
        layout->addWidget(ex_label,0,0,1,2);
        layout->addWidget(lng_label,1,0,1,1);
        layout->addWidget(lng_edit,1,1,1,1);
        layout->addWidget(lat_label,2,0,1,1);
        layout->addWidget(lat_edit,2,1,1,1);
        layout->addWidget(ok_button,3,0,1,1);
        layout->addWidget(cancel_button,3,1,1,1);
        dialog->setLayout(layout);
        dialog->setWindowTitle("经纬度点");
        dialog->move((deskdop->width()-dialog->width())/2,(deskdop->height()-dialog->height())/2);

        connect(ok_button,&QPushButton::clicked,dialog,&QDialog::accept);
        connect(cancel_button,&QPushButton::clicked,dialog,&QDialog::reject);
    }
    int ret = dialog->exec();
    if(ret == QDialog::Accepted){
        double lng = lng_edit->text().toDouble();
        double lat = lat_edit->text().toDouble();
        emit m_webview->obj()->addMarkerAt(lng,lat);
    }
}

void SubWindow::openMapClicked()
{
    if(!this->isActiveWindow())return;
    QStringList l = QFileDialog::getOpenFileNames(this,"打开地图文件","","",nullptr,QFileDialog::DontUseNativeDialog);
    for(int i=0; i<l.length();i++){
        emit m_webview->obj()->openMapJson(l[i]);
    }
}

void SubWindow::openPathClicked()
{
    if(!this->isActiveWindow())return;
    QStringList l = QFileDialog::getOpenFileNames(this,"打开采点路径文件","","",nullptr,QFileDialog::DontUseNativeDialog);
    for(int i=0; i<l.length();i++){
        emit m_webview->obj()->openPathJson(l[i]);
    }
}

void SubWindow::addPerClicked()
{
    if(!this->isActiveWindow())return;
    QStringList l = QFileDialog::getOpenFileNames(this,"打开per文件","","",nullptr,QFileDialog::DontUseNativeDialog);
    for(int i=0; i<l.length();i++){
        string str;
        if( !readFile(l[i].toStdString(),str) )continue;
        timeval tv;
        QJsonObject json;
        gettimeofday(&tv,nullptr);
        msgDecode((uint8_t *)str.data(),str.length(),"",tv,json,false);
        if(!json.isEmpty()){
            emit m_webview->obj()->addAsnJson(json);
        }
    }
}

void SubWindow::clearClicked()
{
    if(!this->isActiveWindow())return;
    emit m_webview->obj()->clearAll();
}
