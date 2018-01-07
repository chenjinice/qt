#ifndef TABLE_H
#define TABLE_H

#define DEFAULT_ROW             10
#define DEFAULT_COL             10
#define SINGLE_QUOT             "&apos;"
#define ROW_SEPARATOR           "@-"
#define COL_SEPARATOR           "@|"
#define STYLE_SEPARATOR         "@:"
#define SPAN_SEPARATOR          "+"
#define SIZE_SEPARATOR          "*"

class QMenu;
class QAction;
class QTableWidgetItem;

#include <QTableWidget>

class MyTable : public QTableWidget
{
public:
    explicit MyTable(QWidget *parent = nullptr);
    ~MyTable();

    // 0：保存内容和格式，1：仅保存格式
    QString getTableData(int type = 0);
    void setTableData(const QString &data_origin);

    // QTableWidget的 rowSpan 在有合并单元格的时候，返回值不符合这个表格需求，因而新建一个函数
    int rSpan(int row, int column);
    // QTableWidget的 columnSpan 在有合并单元格的时候，返回值不符合这个表格需求，因而新建一个函数
    int cSpan(int row, int column);

private :
    QMenu   *m_menu;
    QAction *m_addRowF;
    QAction *m_addRowB;
    QAction *m_deleteRow;
    QAction *m_addColF;
    QAction *m_addColB;
    QAction *m_deleteCol;
    QAction *m_merge;

    void createMenu();
    void fillTable(int rows = DEFAULT_ROW,int cols = DEFAULT_COL);
    QTableWidgetItem *createTableItem();
    int  isSquare();
    void addRow(int index);
    void addCol(int index);

private slots:
    void showContextMenu(const QPoint &pos);
    void addRowF();
    void addRowB();
    void deleteRow();
    void addColF();
    void addColB();
    void deleteCol();
    void merge();
};

#endif // TABLE_H
