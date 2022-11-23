#ifndef LETSTM_H
#define LETSTM_H

#include "expression/expression.h"
#include "statement.h"

class LetStm : public Statement
{
private:
    QString var;
    Expression *exp;
public:
    explicit LetStm(qint32 line, const QString &var, Expression *exp);
    qint32 run(QMap<QString, qint32> &varTable, QTextStream &stream) const override;
    void showTree(QTextStream &stream) const override;
};

#endif // LETSTM_H
