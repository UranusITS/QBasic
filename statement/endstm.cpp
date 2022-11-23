#include "endstm.h"

EndStm::EndStm(qint32 line): Statement(line) {}

qint32 EndStm::run(QMap<QString, qint32> &varTable, QTextStream &stream) const
{
    return -1;
}

void EndStm::showTree(QTextStream &stream) const
{
    stream << line << " END" << Qt::endl;
}
