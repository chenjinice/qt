#ifndef TIMEDIALOG_H
#define TIMEDIALOG_H

class QDateTimeEdit;

#include <QDialog>

class timeDialog : public QDialog
{
    Q_OBJECT
public:
    timeDialog();
    ~timeDialog();

    //返回开始时间和结束时间
    //如果点击的是 ok 按钮，返回的list的0元素是开始时间，1元素是结束时间
    //如果点击的是 cancel 按钮， 返回一个空的list
    QStringList getTime();

public slots:
    void ok_clicked();
    void cancel_clicked();

private:
    QDateTimeEdit  *m_startTimeEdit;
    QDateTimeEdit  *m_endTimeEdit;
    QString         m_format;
};

#endif // TIMEDIALOG_H
