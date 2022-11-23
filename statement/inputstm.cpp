#include "inputstm.h"

InputStm::InputStm(qint32 line, const QString &var): Statement(line), var(var) {}

qint32 InputStm::run(QMap<QString, qint32> &varTable, QTextStream &stream) const
{
    qint32 val;
    stream >> val;
    varTable[var] = val;
    return 0;
}

void InputStm::showTree(QTextStream &stream) const
{
    stream << line << " INPUT" << Qt::endl;
    stream << "    " << var << Qt::endl;
}
