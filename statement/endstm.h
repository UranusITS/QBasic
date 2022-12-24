#ifndef ENDSTM_H
#define ENDSTM_H

#include "statement.h"

class EndStm : public Statement
{
public:
    explicit EndStm(qint32 line, const QString &code);
    qint32 run(QMap<QString, qint32> &varTable, QTextStream &stream) const override;
    void showTree(QTextStream &stream) const override;
};

#endif // ENDSTM_H
