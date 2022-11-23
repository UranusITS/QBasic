#include "conexp.h"

ConExp::ConExp(qint32 num): num(num) {}

qint32 ConExp::value(QMap<QString, qint32> &varTable) const
{
    return num;
}

void ConExp::showTree(QTextStream &stream, qint32 tabs) const
{
    stream << QString("    ").repeated(tabs)
           << num << Qt::endl;
}
