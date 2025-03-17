#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QDebug>

/**
 * @todo write docs
 */
class Log : public QObject
{
    Q_OBJECT

signals:
    void NewLogOutput(const QString & str);

public:
    Log & operator << (const QString & str);
};

Log & log();
Log & bar();
Log & dialog();

#endif // LOG_H
