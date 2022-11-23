#include "ifstm.h"

const QVector<QString> IfStm::optStrings({"=", "<", ">"});

IfStm::IfStm(qint32 line, Operator opt, Expression *expl, Expression *expr, qint32 n): Statement(line), opt(opt), expl(expl), expr(expr), n(n) {}

qint32 IfStm::run(QMap<QString, qint32> &varTable, QTextStream &stream) const
{
    switch (opt) {
    case EQUAL:
        return expl->value(varTable) == expr->value(varTable) ? n : 0;
    case LESS:
        return expl->value(varTable) < expr->value(varTable) ? n : 0;
    case GREATER:
        return expl->value(varTable) > expr->value(varTable) ? n : 0;
    }
    return 0;
}

void IfStm::showTree(QTextStream &stream) const
{
    stream << line << " IF THEN" << Qt::endl;
    stream << "    " << optStrings[opt] << Qt::endl;
    expl->showTree(stream, 1);
    expr->showTree(stream, 1);
    stream << "    " << n << Qt::endl;
}
