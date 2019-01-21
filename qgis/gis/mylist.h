#ifndef MYLIST_H
#define MYLIST_H

#include <QListWidget>

class MyList : public QListWidget
{
    Q_OBJECT
public:
    explicit MyList(QWidget *parent = nullptr);
    ~MyList();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

signals:
    void indexsChanged();

public slots:
};

#endif // MYLIST_H
