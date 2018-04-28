/*
 *
 * 处理 log 的函数
 *
 */

#ifndef CLOG
#define CLOG

class QString;

void c_log(const QString &str);

void c_log(const char *format,...);

#endif // CLOG

