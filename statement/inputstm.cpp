#include "inputstm.h"

InputStm::InputStm(qint32 line, const QString &code, const QString &var): Statement(line, code), var(var) {}

qint32 InputStm::run(QMap<QString, qint32> &varTable, QTextStream &stream) const
{
    Q_UNUSED(varTable);
    stream << " ? ";
    return -2;
}

QString InputStm::getVar() const
{
    return var;
}

void InputStm::showTree(QTextStream &stream) const
{
    stream << line << " INPUT" << Qt::endl;
    stream << "    " << var << Qt::endl;
}
