#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QObject>

class MyDialog : public QObject
{
    Q_OBJECT
public:
    static bool getPoint(double &lat, double &lng);
    static void aboutMe();
private:
    explicit MyDialog(QObject *parent = 0);

};

#endif // MYDIALOG_H
