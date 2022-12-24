#ifndef STATEMENT_H
#define STATEMENT_H

#include <QMap>
#include <QTextStream>
#include <QtGlobal>

class Statement
{
protected:
    qint32 line;
    QString code;
public:
    explicit Statement(qint32 line, const QString &code);
    virtual qint32 run(QMap<QString, qint32> &varTable, QTextStream &stream) const = 0;
    virtual void showTree(QTextStream &stream) const = 0;
    qint32 getLine() const;
    QString getCode() const;
};

#endif // STATEMENT_H
