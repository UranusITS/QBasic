#ifndef INPUTSTM_H
#define INPUTSTM_H

#include "statement.h"

class InputStm : public Statement
{
private:
    QString var;
public:
    explicit InputStm(qint32 line, const QString &code, const QString &var);
    qint32 run(QMap<QString, qint32> &varTable, QTextStream &stream) const override;
    QString getVar() const;
    void showTree(QTextStream &stream) const override;
};

#endif // INPUTSTM_H
