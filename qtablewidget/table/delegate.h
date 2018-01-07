#ifndef MYDELEGATE_H
#define MYDELEGATE_H

class QTableWidget;

#include <QStyledItemDelegate>

class Delegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit Delegate(QTableWidget *parent);
    ~Delegate();

    void paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index) const;


private:
    QTableWidget * m_table;
};

#endif // MYDELEGATE_H
