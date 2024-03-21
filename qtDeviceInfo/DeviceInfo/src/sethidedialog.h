#ifndef SETHIDEDIALOG_H
#define SETHIDEDIALOG_H

#include <QDialog>

class QCheckBox;


class SetHideDialog : public QDialog
{
    Q_OBJECT
public:
    static SetHideDialog *ins();
    ~SetHideDialog();

private:
    SetHideDialog();
    void checkboxChanged(int value);
    void selectAll();

    static SetHideDialog * m_instance;
    QList<QCheckBox *>     m_list;

signals:
    void hideSignal(QString item,bool flag);
};

#endif // SETHIDDENDIALOG_H
