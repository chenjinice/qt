#ifndef WIDGET_H
#define WIDGET_H

class QVBoxLayout;
class MyTable;
class QLineEdit;
class QLabel;

#include <QWidget>

class PlanWidget : public QWidget
{
    Q_OBJECT

public:
    enum SaveType { All=0, Form=1};
    PlanWidget(QWidget *parent = 0);
    ~PlanWidget();

    QString title();
    QString examine();
    QString leader();
    QString teacher();
    QString editTime();
    QString planData();

    void setTitle(const QString &str);
    void setExamine(const QString &str);
    void setLeader(const QString &str);
    void setTeacher(const QString &str);
    void setEditTime(const QString &str);
    void setPlanData(QString &str);
    void printValidData();
    void printallDate();

private:
    QVBoxLayout  *m_layout;
    MyTable      *m_table;
    QLineEdit    *m_title;
    QLineEdit    *m_exam;
    QLineEdit    *m_leader;
    QLineEdit    *m_teacher;
    QLabel       *m_time;
    void closeEvent(QCloseEvent *event);
    void createInfoWidgets();

private slots:
    void save();
    void open();

public slots:

};

#endif // WIDGET_H
