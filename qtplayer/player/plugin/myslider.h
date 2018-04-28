#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>

class MySlider : public QSlider
{
    Q_OBJECT
public:
    explicit MySlider(QWidget *parent = 0);
    ~MySlider();

protected:
    void mousePressEvent(QMouseEvent *ev);

signals:
    void clickedPosition(int pos);
};

#endif // MYSLIDER_H
