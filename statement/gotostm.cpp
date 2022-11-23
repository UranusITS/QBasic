#include "gotostm.h"

GotoStm::GotoStm(qint32 line, qint32 n): Statement(line), n(n) {}

qint32 GotoStm::run(QMap<QString, qint32> &varTable, QTextStream &stream) const
{
    return n;
}

void GotoStm::showTree(QTextStream &stream) const
{
    stream << line << " GOTO" << Qt::endl;
    stream << "    " << n << Qt::endl;
}
