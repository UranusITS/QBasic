#include "statement.h"

Statement::Statement(qint32 line, const QString &code): line(line), code(code) {}

qint32 Statement::getLine() const
{
    return line;
}

QString Statement::getCode() const
{
    return code;
}
