#include "log.h"

static Log staticLog, staticBar, staticDialog;

Log &Log::operator << (const QString &str)
{
    emit NewLogOutput(str);
    return *this;
}

Log &log()
{
    return staticLog;
}

Log &bar()
{
    return staticBar;
}

Log &dialog()
{
    return staticDialog;
}
