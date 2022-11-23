#ifndef PRINTSTM_H
#define PRINTSTM_H

#include "expression/expression.h"
#include "statement.h"

class PrintStm : public Statement
{
private:
    Expression *exp;
public:
    explicit PrintStm(qint32 line, Expression *exp);
    qint32 run(QMap<QString, qint32> &varTable, QTextStream &stream) const override;
    void showTree(QTextStream &stream) const override;
};

#endif // PRINTSTM_H
