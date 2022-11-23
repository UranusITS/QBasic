#include "printstm.h"

PrintStm::PrintStm(qint32 line, Expression *exp): Statement(line), exp(exp) {}

qint32 PrintStm::run(QMap<QString, qint32> &varTable, QTextStream &stream) const
{
    stream << exp->value(varTable) << Qt::endl;
    return 0;
}

void PrintStm::showTree(QTextStream &stream) const
{
    stream << line << " PRINT" << Qt::endl;
    exp->showTree(stream, 1);
}
