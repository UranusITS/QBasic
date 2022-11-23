#include "letstm.h"

LetStm::LetStm(qint32 line, const QString &var, Expression *exp): Statement(line), var(var), exp(exp) {}

qint32 LetStm::run(QMap<QString, qint32> &varTable, QTextStream &stream) const
{
    varTable[var] = exp->value(varTable);
    return 0;
}

void LetStm::showTree(QTextStream &stream) const
{
    stream << line << " LET" << Qt::endl;
    stream << "    " << var << Qt::endl;
    exp->showTree(stream, 1);
}
