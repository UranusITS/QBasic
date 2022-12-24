#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "statement/inputstm.h"
#include <QAction>
#include <QDebug>
#include <QFileDialog>
#include <QLineEdit>
#include <QMap>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , parser(new Parser)
    , inputMode(false)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    setFixedSize(width(), height());
    initUi();
    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUi()
{
    ui->textCode->setReadOnly(true);
    ui->textTree->setReadOnly(true);
}

void MainWindow::initConnect()
{
    connect(parser, &Parser::insertStatement, this, &MainWindow::insertStatement);
    connect(parser, &Parser::deleteStatement, this, &MainWindow::deleteStatement);
    connect(parser, &Parser::runCommand, this, &MainWindow::run);
    connect(parser, &Parser::loadCommand, this, &MainWindow::loadFile);
    connect(parser, &Parser::saveCommand, this, &MainWindow::saveFile);
    connect(parser, &Parser::clearCommand, this, &MainWindow::clearCode);
    connect(parser, &Parser::helpCommand, this, &MainWindow::showHelp);
    connect(parser, &Parser::quitCommand, this, &MainWindow::quit);
    connect(ui->buttonInsert, &QPushButton::clicked, this, &MainWindow::onInput);
    connect(ui->lineCommand, &QLineEdit::returnPressed, this, &MainWindow::onInput);
    connect(ui->actionLoadFile, &QAction::triggered, this, &MainWindow::loadFile);
    connect(ui->actionSaveFile, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::showHelp);
    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui->actionClearCode, &QAction::triggered, this, &MainWindow::clearCode);
    connect(ui->actionRunCode, &QAction::triggered, this, &MainWindow::run);
}

void MainWindow::updateCode()
{
    QString code;
    QTextStream stream(&code);
    for (Statement *stm: Statements)
    {
        stream << stm->getCode() << Qt::endl;
    }
    ui->textCode->setPlainText(code);
}

void MainWindow::updateTree()
{
    QString str;
    QTextStream stream(&str);
    for (Statement *stm: Statements)
    {
        stm->showTree(stream);
    }
    ui->textTree->setText(str);
}

void MainWindow::runBegin()
{
    ui->actionLoadFile->setDisabled(true);
    ui->actionSaveFile->setDisabled(true);
    ui->actionQuit->setDisabled(true);
    ui->actionHelp->setDisabled(true);
    ui->actionClearCode->setDisabled(true);
    ui->actionRunCode->setDisabled(true);
    ui->menuFile->setDisabled(true);
    ui->menuCode->setDisabled(true);
}

void MainWindow::runEnd()
{
    ui->actionLoadFile->setEnabled(true);
    ui->actionSaveFile->setEnabled(true);
    ui->actionQuit->setEnabled(true);
    ui->actionHelp->setDisabled(true);
    ui->actionClearCode->setEnabled(true);
    ui->actionRunCode->setEnabled(true);
    ui->menuFile->setEnabled(true);
    ui->menuCode->setEnabled(true);
}

void MainWindow::insertStatement(qint32 line, Statement *stm)
{
    auto itStm = Statements.begin();
    while (itStm != Statements.end() && (*itStm)->getLine() < line)
    {
        itStm++;
    }
    if (itStm != Statements.end() && (*itStm)->getLine() == line)
    {
        (*itStm) = stm;
    }
    else
    {
        Statements.insert(itStm, stm);
    }
    updateCode();
    updateTree();
}

void MainWindow::deleteStatement(qint32 line)
{
    auto itStm = Statements.begin();
    while (itStm != Statements.end() && (*itStm)->getLine() < line)
    {
        itStm++;
    }
    if (itStm != Statements.end() && (*itStm)->getLine() == line)
    {
        Statements.erase(itStm);
    }
    updateCode();
    updateTree();
}

void MainWindow::run(bool resume)
{
    auto itStm = Statements.begin();
    QMap<QString, qint32> varTable = envVT;
    QString str;
    QTextStream stream(&str);
    bool done = true;
    if (resume)
    {
        while (itStm !=Statements.end() && (*itStm)->getLine() != resumeLine)
        {
            itStm++;
        }
        itStm++;
        varTable = resumeVT;
    }
    else
    {
        runBegin();
    }
    while (itStm != Statements.end())
    {
        qint32 nextLine = (*itStm)->run(varTable, stream);
        if (nextLine == -1)
        {
            break;
        }
        else if (nextLine == -2)
        {
            inputMode = true;
            resumeLine = (*itStm)->getLine();
            inputVar = static_cast<InputStm *>(*itStm)->getVar();
            resumeVT = varTable;
            done = false;
            break;
        }
        else if (!nextLine)
        {
            itStm++;
        }
        else
        {
            itStm = Statements.begin();
            while(itStm != Statements.end() && (*itStm)->getLine() < nextLine)
            {
                itStm++;
            }
            if (itStm != Statements.end() && (*itStm)->getLine() == nextLine)
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
    QString text = ui->textCommand->toPlainText() + str;
    if (done)
    {
        text += "[INF] Main: code run done\n";
        runEnd();
    }
    ui->textCommand->setPlainText(text);
    ui->textCommand->moveCursor(QTextCursor::End);
}

void MainWindow::loadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load Code File", "./", "Basic Code File(*.bas)");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法读取文件：" + fileName);
        return ;
    }
    clearCode();
    QTextStream in(&file);
    QString line;
    QString stringCmd;
    QTextStream streamCmd(&stringCmd);
    while (!in.atEnd())
    {
        line = in.readLine();
        parser->parseLine(line, envVT, streamCmd);
    }
    QString text = ui->textCommand->toPlainText() + stringCmd;
    ui->textCommand->setPlainText(text);
    ui->textCommand->moveCursor(QTextCursor::End);
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Code File", "./", "Basic Code File(*.bas)");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存至文件：" + fileName);
        return ;
    }
    QTextStream out(&file);
    for (Statement *stm: Statements)
    {
        out << stm->getCode() << Qt::endl;
    }
}

void MainWindow::clearCode()
{
    Statements.clear();
    envVT.clear();
    inputMode = false;
    resumeVT.clear();
    updateCode();
    updateTree();
}

void MainWindow::showHelp()
{
    QString str;
    QTextStream stream(&str);
    stream << "-------------  HELP  MESSAGE  -------------" << Qt::endl;
    stream << "| Commands supported in QBasic:           |" << Qt::endl;
    stream << "| RUN      | CTRL + R                     |" << Qt::endl;
    stream << "|     run the code                        |" << Qt::endl;
    stream << "| LOAD     | CTRL + O                     |" << Qt::endl;
    stream << "|     load a code file                    |" << Qt::endl;
    stream << "| SAVE     | CTRL + S                     |" << Qt::endl;
    stream << "|     save the current code to a file     |" << Qt::endl;
    stream << "| CLEAR    | CTRL + C                     |" << Qt::endl;
    stream << "|     clear the current code              |" << Qt::endl;
    stream << "| HELP     | CTRL + H                     |" << Qt::endl;
    stream << "|     show this message                   |" << Qt::endl;
    stream << "| LIST                                    |" << Qt::endl;
    stream << "|     deprecated                          |" << Qt::endl;
    stream << "| QUIT     | ESC                          |" << Qt::endl;
    stream << "|     close this application              |" << Qt::endl;
    stream << " - - - - - - - - - - - - - - - - - - - - - " << Qt::endl;
    stream << "| Statements supported in QBasic:         |" << Qt::endl;
    stream << "| RemStm   | m REM ...                    |" << Qt::endl;
    stream << "| LetStm   | m LET var = exp              |" << Qt::endl;
    stream << "| PrintStm | m PRINT exp                  |" << Qt::endl;
    stream << "| InputStm | m INPUT var                  |" << Qt::endl;
    stream << "| GotoStm  | m GOTO n                     |" << Qt::endl;
    stream << "| IfStm    | m IF exp1 =/</> exp2 THEN n  |" << Qt::endl;
    stream << "| EndStm   | m END                        |" << Qt::endl;
    stream << "-------------------------------------------" << Qt::endl;
    QString text = ui->textCommand->toPlainText() + str;
    ui->textCommand->setPlainText(text);
    ui->textCommand->moveCursor(QTextCursor::End);
}

void MainWindow::quit()
{
    close();
}

void MainWindow::onInput()
{
    if (inputMode)
    {
        qint32 val;
        QString str = ui->lineCommand->text();
        QTextStream stream(&str);
        stream >> val;
        resumeVT[inputVar] = val;
        inputMode = false;
        str = ui->textCommand->toPlainText();
        str += ui->lineCommand->text() + "\n";
        ui->textCommand->setPlainText(str);
        ui->textCommand->moveCursor(QTextCursor::End);
        ui->lineCommand->clear();
        run(true);
    }
    else
    {
        QString str;
        QTextStream stream(&str);
        parser->parseLine(ui->lineCommand->text(), envVT, stream);
        QString text = ui->textCommand->toPlainText() + str;
        ui->textCommand->setPlainText(text);
        ui->textCommand->moveCursor(QTextCursor::End);
        ui->lineCommand->clear();
    }
}
