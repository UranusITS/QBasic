#include "remstm.h"

RemStm::RemStm(qint32 line, const QString &text): Statement(line), text(text) {}

qint32 RemStm::run(QMap<QString, qint32> &varTable, QTextStream &stream) const
{
    return 0;
}

void RemStm::showTree(QTextStream &stream) const
{
    stream << line << " REM" << Qt::endl;
    stream << text << Qt::endl;
}
