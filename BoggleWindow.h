#ifndef BOGGLEWINDOW_H
#define BOGGLEWINDOW_H

#include "Board.h"
#include "Console.h"
#include "WordListWidget.h"
#include <QPushButton>
#include <QMainWindow>
#include "lexicon.h"
#include<QDebug>

class BoggleWindow : public QMainWindow
{
    Q_OBJECT

public:
    BoggleWindow(QWidget *parent = 0);
    ~BoggleWindow();

    QPushButton *shake;
    QPushButton *finish;


    WordListWidget *me;
    WordListWidget *computer;
    Board *board;
    Console *console;

    Lexicon *lex;

    //有关点击游戏的数据成员
    int nextx;
    int nexty;
    QString clickCheck;

    int finishClicked;

    bool search(QString newword,int checknow,Board * board,int x,int y,int visited[][5]);
    bool searchAll(int x,int y,QString nowString,int visited[][5], Board *board);
private:
    static const int BOGGLE_WINDOW_WIDTH = 800;
    static const int BOGGLE_WINDOW_HEIGHT = 600;

public slots:
    void receiveNewWord(QString newline);
    void findAll();
    void clickSearch(int x,int y);
    void clearConsole();
    void clickFinish();


};

#endif // BOGGLEWINDOW_H
