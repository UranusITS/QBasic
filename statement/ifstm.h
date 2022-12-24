#ifndef IFSTM_H
#define IFSTM_H

#include "expression/expression.h"
#include "statement.h"

class IfStm : public Statement
{
public:
    enum Operator {
        EQUAL,
        LESS,
        GREATER
    };
    const static QVector<QString> optStrings;
private:
    Operator opt;
    Expression *expl, *expr;
    qint32 n;
public:
    explicit IfStm(qint32 line, const QString &code, Operator opt, Expression *expl, Expression *expr, qint32 n);
    qint32 run(QMap<QString, qint32> &varTable, QTextStream &stream) const override;
    void showTree(QTextStream &stream) const override;
};

#endif // IFSTM_H
