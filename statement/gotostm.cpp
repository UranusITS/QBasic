#include "gotostm.h"

GotoStm::GotoStm(qint32 line, const QString &code, qint32 n): Statement(line, code), n(n) {}

qint32 GotoStm::run(QMap<QString, qint32> &varTable, QTextStream &stream) const
{
    return n;
}

void GotoStm::showTree(QTextStream &stream) const
{
    stream << line << " GOTO" << Qt::endl;
    stream << "    " << n << Qt::endl;
}
