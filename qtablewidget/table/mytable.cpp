#include "mytable.h"
#include "delegate.h"
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QDebug>

MyTable::MyTable(QWidget *parent)
      : QTableWidget(parent)
{
    this->fillTable(DEFAULT_ROW,DEFAULT_COL);
    this->createMenu();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    Delegate *delegate = new Delegate(this);
    this->setItemDelegate(delegate);
    connect(this,&MyTable::customContextMenuRequested,this,&MyTable::showContextMenu);

//    this->verticalHeader()->setVisible(false);
//    this->horizontalHeader()->setVisible(false);
}

MyTable::~MyTable()
{
    this->clear();
}

QString MyTable::getTableData(int type)
{
    QString str;
    int row = this->rowCount();
    int col = this->columnCount();
    int valid_row = 1;
    int valid_col = 1;

    QList<int> widthList;
    QList<int> heightList;
    for(int i=0;i<row;i++){
        QString line;
        for(int m=0;m<col;m++){
            QTableWidgetItem *item = this->item(i,m);
            if(item /*&& !item->text().isEmpty()*/){
                //第三个数据为 span ，中间用span分隔符分开
                int rowSpan = this->rSpan(i,m);
                int colSpan = this->cSpan(i,m);
                if(rowSpan == 0 && colSpan == 0)continue;
                QString str3_span = "";
//                if(rowSpan > 1 && colSpan > 1){
                    str3_span = QString("%1%2%3").arg(rowSpan).arg(SPAN_SEPARATOR).arg(colSpan);
//                }else if(rowSpan > 1){
//                    str3_span = QString("%1%2").arg(rowSpan).arg(SPAN_SEPARATOR);
//                }else if(colSpan > 1) {
//                    str3_span = QString("%1%2").arg(SPAN_SEPARATOR).arg(colSpan);
//                }
                //第四个数据为 行的宽度和高度，中间用*隔开
                int width = this->columnWidth(m);
                int height = this->rowHeight(i);
                bool w_exist = widthList.contains(m);
                bool h_exist = heightList.contains(i);
                QString str4_size ="";
                if((!w_exist) && (!h_exist)){
                    str4_size = QString("%1%2%3").arg(width).arg(SIZE_SEPARATOR).arg(height);
                    widthList.append(m);
                    heightList.append(i);
                }else if (!w_exist) {
                    str4_size = QString("%1%2").arg(width).arg(SIZE_SEPARATOR);
                    widthList.append(m);
                }else if (!h_exist) {
                    str4_size = QString("%1%2").arg(SIZE_SEPARATOR).arg(height);
                    heightList.append(i);
                }
                //第一二个数据分别为行坐标和列坐标
                if(!line.isEmpty()){
                    line += COL_SEPARATOR;
                }
                line += QString("%1,%2,%3,%4").arg(i).arg(m).arg(str3_span).arg(str4_size);
                if(type == 0){
                    line += STYLE_SEPARATOR;
                    line += item->text();
                }
                if(valid_row < i+rowSpan)valid_row = i+rowSpan;
                if(valid_col < m+colSpan)valid_col = m+colSpan;
            }
        }
        if(!line.isEmpty()){
            str += ROW_SEPARATOR;
            str += line;
        }
    }
    if(!str.isEmpty()){
        QString header =  QString("%1,%2").arg(valid_row).arg(valid_col);
        str.insert(0,header);
    }
    return str.replace("'",SINGLE_QUOT);
}

void MyTable::setTableData(const QString &data_origin)
{
    QString data = data_origin;
    data.replace(SINGLE_QUOT,"'");
    QStringList rows = data.split(ROW_SEPARATOR);
    if(rows.length() <= 1){
        this->fillTable();
        return;
    }

    QStringList header = rows[0].split(",");
    if(header.length() != 2){
        this->fillTable();
        return;
    }

    this->fillTable(header[0].toInt(),header[1].toInt());
    for(int i=1;i<rows.length();i++){
        QStringList cols = rows[i].split(COL_SEPARATOR);
        for(int m=0;m<cols.length();m++){
            QStringList content = cols[m].split(STYLE_SEPARATOR);
//            qDebug() << content;
            if(content.length() < 1)continue;
            QStringList styles = content[0].split(",");
            if(styles.length() < 4)continue;
            int itemRow = styles[0].toInt();
            int itemCol = styles[1].toInt();
            int rowSpan = 1;
            int colSpan = 1;
            {
                QStringList list = styles[2].split(SPAN_SEPARATOR);
                if(list.length() == 2){
                    if(!list[0].isEmpty())rowSpan = list[0].toInt();
                    if(!list[1].isEmpty())colSpan = list[1].toInt();
                }
            }
            {
                QStringList list = styles[3].split(SIZE_SEPARATOR);
                if(list.length() == 2){
                    if(!list[0].isEmpty())this->setColumnWidth(itemCol,list[0].toInt());
                    if(!list[1].isEmpty())this->setRowHeight(itemRow,list[1].toInt());
                }
            }
            QTableWidgetItem *item = this->item(itemRow,itemCol);
            if(rowSpan>1 || colSpan>1)this->setSpan(itemRow,itemCol,rowSpan,colSpan);
            if(content.length() > 1){
                QString text = content[1];
                item->setText(text);
            }
        }
    }

}

int MyTable::rSpan(int row, int column)
{
    int rCount = this->rowCount();
    int cCount = this->columnCount();
    int t_size = rCount * cCount;
    bool *array = new bool[t_size];
    for(int i=0;i<t_size;i++)array[i] = true;

    for(int m=0;m<rCount;m++){
        for(int n=0;n<cCount;n++){
            if(!array[m*cCount+n])continue;
            int rowSpan = this->rowSpan(m,n);
            int colSpan = this->columnSpan(m,n);
            if(rowSpan > 1 || colSpan > 1){
                for(int j=m;j<m+rowSpan;j++){
                    for(int k=n;k<n+colSpan;k++){
                        if(j==m && k==n)continue;
                        array[j*cCount+k] = false;
                    }
                }
            }
        }
    }

    bool value = array[row*cCount+column];
    delete []array;
    array = nullptr;

    if(value)return this->rowSpan(row,column);
    else return 0;
}

int MyTable::cSpan(int row, int column)
{
    int rCount = this->rowCount();
    int cCount = this->columnCount();
    int t_size = rCount * cCount;
    bool *array = new bool[t_size];
    for(int i=0;i<t_size;i++)array[i] = true;

    for(int m=0;m<rCount;m++){
        for(int n=0;n<cCount;n++){
            if(!array[m*cCount+n])continue;
            int rowSpan = this->rowSpan(m,n);
            int colSpan = this->columnSpan(m,n);
            if(rowSpan > 1 || colSpan > 1){
                for(int j=m;j<m+rowSpan;j++){
                    for(int k=n;k<n+colSpan;k++){
                        if(j==m && k==n)continue;
                        array[j*cCount+k] = false;
                    }
                }
            }
        }
    }

    bool value = array[row*cCount+column];
    delete []array;
    array = nullptr;

    if(value)return this->columnSpan(row,column);
    else return 0;
}

void MyTable::createMenu()
{
    m_menu = new QMenu(this);
    m_addRowF = new QAction(tr("前插一行"),this);
    m_addRowB = new QAction(tr("后插一行"),this);
    m_deleteRow = new QAction(tr("删除行"),this);
    m_addColF = new QAction(tr("前插一列"),this);
    m_addColB = new QAction(tr("后插一列"),this);
    m_deleteCol = new QAction(tr("删除列"),this);
    m_merge = new QAction(tr("合并"));

    connect(m_addRowF,&QAction::triggered,this,&MyTable::addRowF);
    connect(m_addRowB,&QAction::triggered,this,&MyTable::addRowB);
    connect(m_deleteRow,&QAction::triggered,this,&MyTable::deleteRow);
    connect(m_addColF,&QAction::triggered,this,&MyTable::addColF);
    connect(m_addColB,&QAction::triggered,this,&MyTable::addColB);
    connect(m_deleteCol,&QAction::triggered,this,&MyTable::deleteCol);
    connect(m_merge,&QAction::triggered,this,&MyTable::merge);

    m_menu->addAction(m_addRowF);
    m_menu->addAction(m_addRowB);
    m_menu->addAction(m_deleteRow);
    m_menu->addAction(m_addColF);
    m_menu->addAction(m_addColB);
    m_menu->addAction(m_deleteCol);
    m_menu->addAction(m_merge);
}

void MyTable::fillTable(int rows, int cols)
{
    this->setColumnCount(0);
    this->setRowCount(0);
    if(rows>100 || rows<1)rows = DEFAULT_ROW;
    if(cols>100 || cols<1)cols = DEFAULT_COL;
    this->setColumnCount(cols);
    this->setRowCount(rows);
    for(int m=0;m<rows;m++){
        for(int n=0;n<cols;n++){
            QTableWidgetItem *item = this->createTableItem();
            this->setItem(m,n,item);
        }
    }
}

QTableWidgetItem *MyTable::createTableItem()
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setTextAlignment(Qt::AlignCenter);
    QFont font;
    font.setPixelSize(11);
    item->setFont(font);
    return item;
}

int MyTable::isSquare()
{
    QList<QTableWidgetItem*> list = this->selectedItems();
    int length = list.length();
    if(length == 0)return length;

    int firstRow = list[0]->row();
    int firstCol = list[0]->column();
    int maxRow = firstRow + this->rowSpan(firstRow,firstCol) - 1;
    int minRow = firstRow;
    int maxCol = firstCol + this->columnSpan(firstRow,firstCol) - 1;
    int minCol = firstCol;
    int count = 0;

    for(int i=0;i<list.length();i++){
        QTableWidgetItem *item = list[i];
        int row = item->row();
        int col = item->column();
        int rowSpan = this->rowSpan(row,col);
        int colSpan = this->columnSpan(row,col);
//        qDebug() << row << "," << col;
        if(maxRow < row + rowSpan - 1)maxRow = row + rowSpan - 1;
        if(minRow > row)minRow = row;
        if(maxCol < col + colSpan - 1)maxCol = col + colSpan - 1;
        if(minCol > col)minCol = col;
        count += rowSpan*colSpan;
    }

    int square = (maxRow-minRow+1) * (maxCol-minCol+1);
//    qDebug("%d-%d+1 * %d-%d+1 , square : %d, length : %d,count : %d",maxRow,minRow,maxCol,minCol,square,length,count);
    if(square == count){ //是矩形
        return length;
    }else {
        return -1;
    }
}

void MyTable::addRow(int index)
{
    this->insertRow(index);
    for(int n=0;n<this->columnCount();n++){
        QTableWidgetItem *item = this->createTableItem();
        this->setItem(index,n,item);
//        item->setText(QString("%1,%2").arg(index).arg(n));
    }
}

void MyTable::addCol(int index)
{
    this->insertColumn(index);
    for(int n=0;n<this->rowCount();n++){
        QTableWidgetItem *item = this->createTableItem();
        this->setItem(n,index,item);
//        item->setText(QString("%1,%2").arg(index).arg(n));
    }
}

void MyTable::showContextMenu(const QPoint &pos)
{
    int length = this->isSquare();
    if(length <= 0){
        m_addRowF->setEnabled(false);
        m_addRowB->setEnabled(false);
        m_deleteRow->setEnabled(false);
        m_addColF->setEnabled(false);
        m_addColB->setEnabled(false);
        m_deleteCol->setEnabled(false);
        m_merge->setEnabled(false);
    }else if(length == 1){
        m_addRowF->setEnabled(true);
        m_addRowB->setEnabled(true);
        m_deleteRow->setEnabled(true);
        m_addColF->setEnabled(true);
        m_addColB->setEnabled(true);
        m_deleteCol->setEnabled(true);
        m_merge->setEnabled(false);
    }else if(length > 1) {
        m_addRowF->setEnabled(false);
        m_addRowB->setEnabled(false);
        m_deleteRow->setEnabled(false);
        m_addColF->setEnabled(false);
        m_addColB->setEnabled(false);
        m_deleteCol->setEnabled(false);
        m_merge->setEnabled(true);
    }
    m_menu->exec(QCursor::pos());
}

void MyTable::addRowF()
{
    this->addRow(this->currentRow());
}

void MyTable::addRowB()
{
    int row = this->currentRow();
    int col = this->currentColumn();
    int rowSpan = this->rowSpan(row,col);
    this->addRow(this->currentRow()+rowSpan);
}

void MyTable::deleteRow()
{
    int index = this->currentRow();
    this->removeRow(index);
}

void MyTable::addColF()
{
    this->addCol(this->currentColumn());
}

void MyTable::addColB()
{
    int row = this->currentRow();
    int col = this->currentColumn();
    int colSpan = this->columnSpan(row,col);
    this->addCol(this->currentColumn()+colSpan);
}

void MyTable::deleteCol()
{
    int index = this->currentColumn();
    this->removeColumn(index);
}

void MyTable::merge()
{
    QList<QTableWidgetItem*> list = this->selectedItems();
    int length = list.length();
    if(length < 1)return;

    int firstRow = list[0]->row();
    int firstCol = list[0]->column();
    int maxRow = firstRow + this->rowSpan(firstRow,firstCol) - 1;
    int minRow = firstRow;
    int maxCol = firstCol + this->columnSpan(firstRow,firstCol) - 1;
    int minCol = firstCol;

    for(int i=0;i<list.length();i++){
        QTableWidgetItem *item = list[i];
        int row = item->row();
        int col = item->column();
        int rowSpan = this->rowSpan(row,col);
        int colSpan = this->columnSpan(row,col);
        if(maxRow < row + rowSpan - 1)maxRow = row + rowSpan - 1;
        if(minRow > row)minRow = row;
        if(maxCol < col + colSpan - 1)maxCol = col + colSpan - 1;
        if(minCol > col)minCol = col;

        if(rowSpan != 1 || colSpan != 1){
            this->setSpan(row,col,1,1);
        }
    }

    for(int i=minRow;i<maxRow+1;i++){
        for(int k=minCol;k<maxCol+1;k++){
            QTableWidgetItem *item = this->item(i,k);
            if(item && !(minRow == i && minCol==k))item->setText("");
        }
    }
    this->setSpan(minRow,minCol,maxRow-minRow+1,maxCol-minCol+1);
}

