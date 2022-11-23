#ifndef CONEXP_H
#define CONEXP_H

#include "expression.h"

class ConExp : public Expression
{
private:
    qint32 num;
public:
    explicit ConExp(qint32 num);
    qint32 value(QMap<QString, qint32> &varTable) const override;
    void showTree(QTextStream &stream, qint32 tabs) const override;
};

#endif // CONEXP_H
