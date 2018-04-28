#include "clog.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include <QMutex>
#include <QDateTime>
#include<stdarg.h>

void saveToFile(const QString &str)
{
    QString path ="/home/chen/Desktop/log/"+QDate::currentDate().toString("yyyy_MM_dd");
    QString time = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss] ");
    QString log = time +str + "\n";
    qDebug() << log;
    QFile file(path);
    if(!file.open(QIODevice::Append|QIODevice::Text)){
        qDebug() << "open log file error";
        return;
    }
    QTextStream out(&file);
    out << log;
}

void c_log(const QString &str)
{
    saveToFile(str);
}

void c_log(const char *format,...)
{
    va_list ap;
    const char *p;
    char *sval;
    int ival;
    int64_t i64val;
    double dval;
    QString str;
    QString temp;

    va_start(ap, format);
    for (p = format; *p; p++) {
        if(*p != '%') {
            str.append(*p);
            continue;
        }
        switch(*++p) {
        case 'd':
            ival = va_arg(ap, int);
            str.append(QString::number(ival));
            break;
        case 'f':
            dval = va_arg(ap, double);
            str.append(QString::number(dval));
            break;
        case 's':
            for (sval = va_arg(ap, char *); *sval; sval++)
            str.append(sval);
            break;
        case 'p':
            ival = va_arg(ap,int);
            temp.sprintf("%p",ival);
            str.append(temp);
            break;
        case 'l':
            if(*++p =='d'){
                i64val = va_arg(ap,int64_t);
                temp.sprintf("%ld",i64val);
                str.append(temp);
            }else {
                str.append("%l");
            }
            break;
        default:
            break;
        }
    }
    va_end(ap);
    saveToFile(str);
}

