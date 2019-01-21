/*
 *
 *
 *
 * chen jin
 */

#ifndef MYTREE_H
#define MYTREE_H

class QFileSystemModel;
class QStringList;

#include <QTreeView>

class MyTree : public QTreeView
{
    Q_OBJECT

public:
    MyTree(QWidget *parent = nullptr);
    ~MyTree();

    QStringList *selectedItems();

private:
    QFileSystemModel *m_model;
    QStringList      m_list;

public slots:
    void treeMenu(QPoint p);
    void addFiles();

signals:
    void addAction_clicked();
};

#endif // MYTREE_H
