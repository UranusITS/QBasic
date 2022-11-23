#ifndef OPTEXP_H
#define OPTEXP_H

#include "expression.h"
#include <QVector>

class OptExp : public Expression
{
public:
    enum Operator {
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        POWER
    };
    const static QVector<QString> optStrings;
private:
    Operator opt;
    Expression *expl, *expr;
public:
    explicit OptExp(Operator opt, Expression *expl, Expression *expr);
    qint32 value(QMap<QString, qint32> &varTable) const override;
    void showTree(QTextStream &stream, qint32 tabs) const override;
};

#endif // OPTEXP_H
