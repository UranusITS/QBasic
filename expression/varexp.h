#ifndef VAREXP_H
#define VAREXP_H

#include "expression.h"

class VarExp : public Expression
{
private:
    QString var;
public:
    explicit VarExp(const QString &var);
    qint32 value(QMap<QString, qint32> &varTable) const override;
    void showTree(QTextStream &stream, qint32 tabs) const override;
};

#endif // VAREXP_H
