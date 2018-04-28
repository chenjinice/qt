#include "myslider.h"
#include <QMouseEvent>
#include <QDebug>

MySlider::MySlider(QWidget *parent)
        : QSlider(Qt::Horizontal,parent)
{

}

MySlider::~MySlider()
{

}

void MySlider::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton)	//判断左键
    {
        int dur = this->maximum() - this->minimum()+1;
        int pos = this->minimum() + dur * ((double)ev->x()/this->width());
        if(pos != this->sliderPosition())
        {
            this->setValue(pos);
            emit clickedPosition(pos);
        }
    }
}

