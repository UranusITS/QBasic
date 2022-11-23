#include "varexp.h"

VarExp::VarExp(const QString &var): var(var) {}

qint32 VarExp::value(QMap<QString, qint32> &varTable) const
{
    return varTable[var];
}

void VarExp::showTree(QTextStream &stream, qint32 tabs) const
{
    stream << QString("    ").repeated(tabs)
           << var << Qt::endl;
}
