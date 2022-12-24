#include "parser.h"
#include "expression/conexp.h"
#include "expression/optexp.h"
#include "expression/varexp.h"
#include "statement/endstm.h"
#include "statement/gotostm.h"
#include "statement/ifstm.h"
#include "statement/inputstm.h"
#include "statement/letstm.h"
#include "statement/printstm.h"
#include "statement/remstm.h"

#include <QDebug>

Parser::Parser():
    inputMode(false)
{}

bool Parser::isVar(const QString &str)
{
    if (!str[0].isLetter() && str[0] != '_')
    {
        return false;
    }
    for (QChar ch: str)
    {
        if (!ch.isLetterOrNumber() && ch != '_')
        {
            return false;
        }
    }
    return true;
}

qint32 Parser::eatInt(QString &str)
{
    qint32 pos = 0;
    if (str[pos] == '-')
    {
        pos++;
    }
    while (pos < str.length() && str[pos].isDigit())
    {
        pos++;
    }
    QString num = str.first(pos);
    if (!pos || (pos == 1 && str[0] == '-'))
    {
        return -1;
    }
    str = str.sliced(pos).simplified();
    return num.toInt();
}

Expression *Parser::parseExp(QString &str)
{
    qDebug()<<str;
    qint32 level = 0;
    qint32 pos = -1;
    OptExp::Operator opt;
    for (qint32 i = 0; i < str.length(); i++)
    {
        QChar ch = str[i];
        if (ch == '(')
        {
            level++;
        }
        else if (ch == ')')
        {
            level--;
            if (level < 0)
            {
                return nullptr;
            }
        }
        else if (!level)
        {
            if (ch == '+')
            {
                opt = OptExp::PLUS;
                pos = i;
            }
            else if (ch == '-' && (i && str[i - 1] != '('))
            {
                opt = OptExp::MINUS;
                pos = i;
            }
        }
    }
    if (level)
    {
        return nullptr;
    }
    if (pos == -1)
    {
        for (qint32 i = 0; i < str.length(); i++)
        {
            QChar ch = str[i];
            if (ch == '(')
            {
                level++;
            }
            else if (ch == ')')
            {
                level--;
            }
            else if (!level)
            {
                if (ch == '*' && (i == str.length() - 1 || str[i + 1] != '*') && (!i || str[i - 1] != '*'))
                {
                    opt = OptExp::MULTIPLY;
                    pos = i;
                }
                else if (ch == '/')
                {
                    opt = OptExp::DIVIDE;
                    pos = i;
                }
            }
        }
    }
    if (pos == -1)
    {
        for (qint32 i = 0; i < str.length(); i++)
        {
            QChar ch = str[i];
            if (ch == '(')
            {
                level++;
            }
            else if (ch == ')')
            {
                level--;
            }
            else if (!level && i != str.length() - 1)
            {
                if (ch == '*' && str[i + 1] == '*')
                {
                    opt = OptExp::POWER;
                    pos = i;
                    QString strl = str.first(pos).simplified();
                    QString strr = str.sliced(pos + 2).simplified();
                    Expression *expl = parseExp(strl);
                    Expression *expr = parseExp(strr);
                    if (expl && expr)
                    {
                        return new OptExp(opt, expl, expr);
                    }
                    else
                    {
                        return nullptr;
                    }
                }
            }
        }
    }
    if (pos == -1)
    {
        if (str.front() == '(' && str.back() == ')')
        {
            str = str.sliced(1, str.length() - 2);
            return parseExp(str);
        }
        else if (isVar(str))
        {
            return new VarExp(str);
        }
        bool ok;
        qint32 num = str.toInt(&ok);
        if (ok)
        {
            return new ConExp(num);
        }
    }
    else
    {
        QString strl = str.first(pos).simplified();
        QString strr = str.sliced(pos + 1).simplified();
        Expression *expl = parseExp(strl);
        Expression *expr = parseExp(strr);
        if (expl && expr)
        {
            return new OptExp(opt, expl, expr);
        }
        else
        {
            return nullptr;
        }
    }
    return nullptr;
}

void Parser::parseLine(const QString &line, QMap<QString, qint32> &varTable, QTextStream &stream)
{
    if (inputMode)
    {
        bool ok;
        qint32 val = line.toInt(&ok);
        if (ok)
        {
            stream << val << Qt::endl;
            varTable[inputVar] = val;
        }
        else
        {
            stream << "[ERR] Parser: " << line << " is not a legal value" << Qt::endl;
        }
        inputMode = false;
        return ;
    }
    QString str = line.simplified();
    if (str == "RUN")
    {
        // stream << "[INF] Parser: run code" << Qt::endl;
        emit runCommand();
        return ;
    }
    else if (str == "LOAD")
    {
        stream << "[INF] Parser: load code file" << Qt::endl;
        emit loadCommand();
        return ;
    }
    else if (str == "SAVE")
    {
        stream << "[INF] Parser: save code file" << Qt::endl;
        emit saveCommand();
        return ;
    }
    else if (str == "CLEAR")
    {
        stream << "[INF] Parser: clear code" << Qt::endl;
        emit clearCommand();
        return ;
    }
    else if (str == "HELP")
    {
        stream << "[INF] Parser: show help message" << Qt::endl;
        emit helpCommand();
        return ;
    }
    else if (str == "LIST")
    {
        return ;
    }
    else if (str == "QUIT")
    {
        stream << "[INF] Parser: quit ide" << Qt::endl;
        emit quitCommand();
        return ;
    }
    else if (str.startsWith("LET "))
    {
        str = str.sliced(4).simplified();
        qint32 pos = str.indexOf('=');
        if (pos != -1)
        {
            QString var = str.first(pos).simplified();
            if (isVar(var))
            {
                str = str.sliced(pos + 1).simplified();
                Expression *exp = parseExp(str);
                if (exp)
                {
                    qint32 val = exp->value(varTable);
                    varTable[var] = val;
                    stream << "[INF] Parser: LET command set " << var << " = " << val << Qt::endl;
                }
                else
                {
                    stream << "[ERR] Parser: cannot parse expression \"" << str << "\"" << Qt::endl;
                }
            }
            else
            {
                stream << "[ERR] Parser: \"" << var << "\" is not a legal variable name" << Qt::endl;
            }
        }
        else
        {
            stream << "[ERR] Parser: cannot find \"=\" from LET Statement" << Qt::endl;
        }
        return ;
    }
    else if (str.startsWith("PRINT "))
    {
        str = str.sliced(6).simplified();
        Expression *exp = parseExp(str);
        if (exp)
        {
            stream << "[INF] Parser: PRINT command" << Qt::endl;
            stream << exp->value(varTable) << Qt::endl;
        }
        else
        {
            stream << "[ERR] Parser: cannot parse expression \"" << str << "\"" << Qt::endl;
        }
        return ;
    }
    else if (str.startsWith("INPUT "))
    {
        str = str.sliced(6).simplified();
        if (isVar(str))
        {
            stream << "[INF] Parser:  INPUT Command of var " << str << Qt::endl;
            stream << " ? ";
            inputMode = true;
            inputVar = str;
        }
        else
        {
            stream << "[ERR] Parser: \"" << str << "\" is not a legal variable name" << Qt::endl;
        }
        return ;
    }
    qint32 n = eatInt(str);
    if (n == -1)
    {
        stream << "[ERR] Parser: unknown command " << line << Qt::endl;
        return ;
    }
    else if (n > 1000000 || n < 0)
    {
        stream << "[ERR] Parser: illegal line number " << n << Qt::endl;
        return ;
    }
    if (str.isEmpty())
    {
        stream << "[INF] Parser: delete code of Line " << n << Qt::endl;
        emit deleteStatement(n);
        return ;
    }
    if (str.startsWith("REM "))
    {
        stream << "[INF] Parser: insert new RemStm" << Qt::endl;
        emit insertStatement(n, new RemStm(n, line, str.sliced(4).simplified()));
        return ;
    }
    else if (str.startsWith("LET "))
    {
        str = str.sliced(4).simplified();
        qint32 pos = str.indexOf('=');
        if (pos != -1)
        {
            QString var = str.first(pos).simplified();
            if (isVar(var))
            {
                str = str.sliced(pos + 1).simplified();
                Expression *exp = parseExp(str);
                if (exp)
                {
                    stream << "[INF] Parser: insert new LetStm" << Qt::endl;
                    emit insertStatement(n, new LetStm(n, line, var, exp));
                }
                else
                {
                    stream << "[ERR] Parser: cannot parse expression \"" << str << "\"" << Qt::endl;
                }
            }
            else
            {
                stream << "[ERR] Parser: \"" << var << "\" is not a legal variable name" << Qt::endl;
            }
        }
        else
        {
            stream << "[ERR] Parser: cannot find \"=\" from LET Statement" << Qt::endl;
        }
        return ;
    }
    else if (str.startsWith("PRINT "))
    {
        str = str.sliced(6).simplified();
        Expression *exp = parseExp(str);
        if (exp)
        {
            stream << "[INF] Parser: insert new PrintStm" << Qt::endl;
            emit insertStatement(n, new PrintStm(n, line, exp));
        }
        else
        {
            stream << "[ERR] Parser: cannot parse expression \"" << str << "\"" << Qt::endl;
        }
        return ;
    }
    else if (str.startsWith("INPUT "))
    {
        str = str.sliced(6).simplified();
        if (isVar(str))
        {
            stream << "[INF] Parser: insert new InputStm" << Qt::endl;
            emit insertStatement(n, new InputStm(n, line, str));
        }
        else
        {
            stream << "[ERR] Parser: \"" << str << "\" is not a legal variable name" << Qt::endl;
        }
        return ;
    }
    else if (str.startsWith("GOTO "))
    {
        str = str.sliced(5).simplified();
        qint32 m = eatInt(str);
        if (m == -1)
        {
            stream << "[ERR] Parser: \"" << str << "\" is not a legal number" << Qt::endl;
            return ;
        }
        stream << "[INF] Parser: insert new GotoStm" << Qt::endl;
        emit insertStatement(n, new GotoStm(n, line, m));
    }
    else if (str.startsWith("IF "))
    {
        str = str.sliced(3).simplified();
        qint32 thenPos = str.indexOf("THEN");
        if (thenPos != -1)
        {
            QString mstr = str.sliced(thenPos + 4).simplified();
            bool ok = true;
            qint32 m = mstr.toInt(&ok);
            if (ok)
            {
                str = str.first(thenPos).simplified();
                IfStm::Operator opt;
                qint32 sgnPos = str.indexOf('=');
                if (sgnPos != -1)
                {
                    opt = IfStm::EQUAL;
                }
                else
                {
                    sgnPos = str.indexOf('<');
                    if (sgnPos != -1)
                    {
                        opt = IfStm::LESS;
                    }
                    else
                    {
                        sgnPos = str.indexOf('>');
                        if (sgnPos != -1)
                        {
                            opt = IfStm::GREATER;
                        }
                    }
                }
                if (sgnPos != -1)
                {
                    QString strl = str.first(sgnPos).simplified();
                    QString strr = str.sliced(sgnPos + 1).simplified();
                    Expression *expl = parseExp(strl);
                    Expression *expr = parseExp(strr);
                    if (expl && expr)
                    {
                        stream << "[INF] Parser: insert new IfStm" << Qt::endl;
                        emit insertStatement(n, new IfStm(n, line, opt, expl, expr, m));
                    }
                    else
                    {
                        stream << "[ERR] Parser: cannot parse expression \"" << strl << "\" or \"" << strr << "\"" << Qt::endl;
                    }
                }
                else
                {
                    stream << "[ERR] Parser: cannot find \"=/</>\" from IF Statement" << Qt::endl;
                }
            }
        }
        else
        {
            stream << "[ERR] Parser: cannot find \"THEN\" from IF Statement" << Qt::endl;
        }
    }
    else if (str == "END")
    {
        stream << "[INF] Parser: insert new EndStm" << Qt::endl;
        emit insertStatement(n, new EndStm(n, line));
    }
    else
    {
        stream << "[ERR] Parser: unknown statement " << line << Qt::endl;
    }
}
