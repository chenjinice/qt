#ifndef GISWINDOW_H
#define GISWINDOW_H


class QgsMapCanvas;
class MyTree;
class QgsMapCanvasLayer;
class MyList;
class QListWidgetItem;
class MapConfig;

#include <QWidget>

class GisWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GisWindow(QWidget *parent = nullptr);
    ~GisWindow();

private:
    void addLayer(QString path);
    void listAddItem(QString path);

    QList<QgsMapCanvasLayer> m_mapLaySet;
    QList<MapConfig>         m_mapConfigList;
    QStringList              m_openedFile;
    MyList                  *m_listWidget;
    QgsMapCanvas            *m_mapCanvas;
    MyTree                  *m_treeView;

signals:

public slots:
    void add_slot();
    void test(QListWidgetItem *);
    void indexChanged_slot();
};

#endif // GISWINDOW_H
