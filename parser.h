#ifndef PARSER_H
#define PARSER_H

#include "expression/expression.h"
#include "statement/statement.h"
#include <QObject>

class Parser: public QObject
{
    Q_OBJECT
private:
    bool inputMode;
    QString inputVar;
    bool isVar(const QString &str);
    qint32 eatInt(QString &str);
    Expression *parseExp(QString &str);
public:
    Parser();
    void parseLine(const QString &line, QMap<QString, qint32> &varTable, QTextStream &streamCmd);
signals:
    void insertStatement(qint32 n, Statement *stm);
    void deleteStatement(qint32 n);
    void runCommand(bool resume=false);
    void loadCommand();
    void saveCommand();
    void clearCommand();
    void helpCommand();
    void quitCommand();
};

#endif // PARSER_H
