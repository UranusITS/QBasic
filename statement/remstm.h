#ifndef REMSTM_H
#define REMSTM_H

#include "statement.h"

class RemStm : public Statement
{
private:
    QString text;
public:
    explicit RemStm(qint32 line, const QString &text);
    qint32 run(QMap<QString, qint32> &varTable, QTextStream &stream) const override;
    void showTree(QTextStream &stream) const override;
};

#endif // REMSTM_H
