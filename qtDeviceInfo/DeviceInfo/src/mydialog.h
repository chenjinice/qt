#ifndef MYDIALOG_H
#define MYDIALOG_H


class QLabel;
class QLineEdit;


#include <QDialog>
#include "common.h"

class MyDialog : public QDialog
{
    Q_OBJECT
public:
    static MyDialog *ins();
    ~MyDialog();

private:
    MyDialog();
    void    yesClicked();
    void    noClicked();
    QString getSavedValue(const QString &key);
    void    changedSlot(const QString &);

    static MyDialog *   m_instance;
    QLineEdit *         m_ip1;
    QLineEdit *         m_ip2;
    QLineEdit *         m_ip3;
    QLineEdit *         m_ip4;
    QLabel *            m_to1;
    QLabel *            m_to2;
    QLabel *            m_to3;
    QLineEdit *         m_to4;



signals:
    void addIp(QString ip);
};


#endif // MYDIALOG_H
