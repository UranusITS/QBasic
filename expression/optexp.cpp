#include "optexp.h"
#include <QtMath>

const QVector<QString> OptExp::optStrings({"+", "-", "*", "/", "**"});

OptExp::OptExp(Operator opt, Expression *expl, Expression *expr): opt(opt), expl(expl), expr(expr) {}

qint32 OptExp::value(QMap<QString, qint32> &varTable) const
{
    switch (opt) {
    case PLUS:
        return expl->value(varTable) + expr->value(varTable);
    case MINUS:
        return expl->value(varTable) - expr->value(varTable);
    case MULTIPLY:
        return expl->value(varTable) * expr->value(varTable);
    case DIVIDE:
        return expl->value(varTable) / expr->value(varTable);
    case POWER:
        return qPow(expl->value(varTable), expr->value(varTable));
    }
}

void OptExp::showTree(QTextStream &stream, qint32 tabs) const
{
    stream << QString("    ").repeated(tabs)
           << optStrings[opt] << Qt::endl;
    expl->showTree(stream, tabs + 1);
    expr->showTree(stream, tabs + 1);
}
