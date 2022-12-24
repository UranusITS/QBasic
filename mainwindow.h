#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "parser.h"
#include "statement/statement.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    Parser *parser;
    QList<Statement *> Statements;
    QMap<QString, qint32> envVT;
    bool inputMode;
    qint32 resumeLine;
    QString inputVar;
    QMap<QString, qint32> resumeVT;
    void initUi();
    void initConnect();
    void updateCode();
    void updateTree();
    void runBegin();
    void runEnd();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void insertStatement(qint32 line, Statement *stm);
    void deleteStatement(qint32 line);
    void run(bool resume=false);
    void loadFile();
    void saveFile();
    void clearCode();
    void showHelp();
    void quit();
    void onInput();
};
#endif // MAINWINDOW_H
