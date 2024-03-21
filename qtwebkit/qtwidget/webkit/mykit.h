#ifndef MYKIT_H
#define MYKIT_H

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;
class QThread;
class QMutex;

#include <QObject>
#include <QList>


class MyKit : public QObject
{
    Q_OBJECT
public:
    typedef struct _Img{
        int x;
        int y;
        int z;
        int status;
    }ImgInfo;
    typedef struct _Range{
        int max;
        int min;
    }Range;

    explicit MyKit();
    ~MyKit();

private:
    void threadRun();
    bool connectToGoogle();
    void listRemove(ImgInfo &m);
    bool listFind(ImgInfo &m);
    void listAdd(Range &x, Range &y, int z);
    void listClear();
    double moduleOperation(double num,int min,int max);
    void getX(double lng1, double lng2, int z, Range &r);
    void getY(double lat1, double lat2, int z, Range &r);
    void sendUpdateSignal();
    void myLog(const QString &str);

    bool    m_google;
    bool    m_ready;
    ImgInfo m_img;
    QMutex  *m_mutex;
    QThread *m_thread;
    QList<ImgInfo> m_list;
    QNetworkRequest       *m_request;
    QNetworkAccessManager *m_manager;

private slots:
    void downloadFinish(QNetworkReply *reply);

public slots:
    void get_bounds(double lat1, double lat2, double lng1, double lng2, int zoom);

signals:
    void mapUpdate();
    void uiLog(QString str);
};

#endif // MYKIT_H
