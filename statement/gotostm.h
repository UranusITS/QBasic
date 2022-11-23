#ifndef GOTOSTM_H
#define GOTOSTM_H

#include "statement.h"

class GotoStm : public Statement
{
private:
    qint32 n;
public:
    explicit GotoStm(qint32 line, qint32 n);
    qint32 run(QMap<QString, qint32> &varTable, QTextStream &stream) const override;
    void showTree(QTextStream &stream) const override;
};

#endif // GOTOSTM_H
