#ifndef STATEMENT_H
#define STATEMENT_H

#include <QMap>
#include <QTextStream>
#include <QtGlobal>

class Statement
{
protected:
    qint32 line;
public:
    virtual qint32 run(QMap<QString, qint32> &varTable, QTextStream &stream) const = 0;
    virtual void showTree(QTextStream &stream) const = 0;
    explicit Statement(qint32 line);
};

#endif // STATEMENT_H
