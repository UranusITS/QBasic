#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QMap>
#include <QTextStream>
#include <QtGlobal>

class Expression
{
public:
    virtual qint32 value(QMap<QString, qint32> &varTable) const = 0;
    virtual void showTree(QTextStream &stream, qint32 tabs) const = 0;
};

#endif // EXPRESSION_H
