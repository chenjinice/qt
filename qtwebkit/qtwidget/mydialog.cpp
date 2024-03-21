#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QDesktopWidget>
#include <QApplication>
#include <QMovie>
#include "mydialog.h"

bool MyDialog::getPoint(double &lat, double &lng)
{
    static QDialog *dialog = NULL;
    static QLineEdit *lat_edit = new QLineEdit;
    static QLineEdit *lng_edit = new QLineEdit;
    static QDesktopWidget *deskdop = QApplication::desktop();

    if(dialog == NULL){
        dialog = new QDialog;
        QGridLayout *layout = new QGridLayout;
        QLabel *lat_label = new QLabel("lat");
        QLabel *lng_label = new QLabel("lng");
        QPushButton *ok_button = new QPushButton("Ok");
        QPushButton *cancel_button = new QPushButton("Cancel");
        layout->addWidget(lat_label,0,0,1,1);
        layout->addWidget(lat_edit,0,1,1,1);
        layout->addWidget(lng_label,1,0,1,1);
        layout->addWidget(lng_edit,1,1,1,1);
        layout->addWidget(ok_button,2,0,1,1);
        layout->addWidget(cancel_button,2,1,1,1);
        dialog->setLayout(layout);
        dialog->setWindowTitle("Get Point");
        dialog->move((deskdop->width()-dialog->width())/2,(deskdop->height()-dialog->height())/2);

        connect(ok_button,&QPushButton::clicked,dialog,&QDialog::accept);
        connect(cancel_button,&QPushButton::clicked,dialog,&QDialog::reject);
    }
    int ret = dialog->exec();
    if(ret == QDialog::Accepted){
        lat = lat_edit->text().toDouble();
        lng = lng_edit->text().toDouble();
        return true;
    }else{
        return false;
    }
}

void MyDialog::aboutMe()
{
    static QDialog *dialog = NULL;
    static QDesktopWidget *deskdop = QApplication::desktop();
    if(dialog == NULL){
        dialog = new QDialog;
        QVBoxLayout *layout = new QVBoxLayout;
        QLabel *name = new QLabel(" ChenJin");
        name->setAlignment(Qt::AlignCenter);
        QLabel *picture = new QLabel;
        QMovie *gif = new QMovie(":/me/me.gif");
        picture->setMovie(gif);
        QPushButton *button = new QPushButton("OK");
        gif->start();

        layout->addWidget(name);
        layout->addWidget(picture);
        layout->addWidget(button);
        dialog->setLayout(layout);
        dialog->setWindowTitle("About Me");
        dialog->move((deskdop->width()-dialog->width())/2,(deskdop->height()-dialog->height())/2);

        connect(button,&QPushButton::clicked,dialog,&QDialog::close);
    }
    dialog->show();
}

MyDialog::MyDialog(QObject *parent) : QObject(parent)
{

}

