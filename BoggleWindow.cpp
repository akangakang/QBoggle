#include "BoggleWindow.h"
#include "lexicon.h"
#include <QTime>
#include <QEventLoop>
#include <QFile>
#include <QHBoxLayout>
#include <QTextEdit>
#include <iostream>
#include <QTimer>

BoggleWindow::BoggleWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setWindowTitle("QBoggle!");
    this->setFixedSize(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);

    me = new WordListWidget(this, "Me");
    computer = new WordListWidget(this, "Computer");
    board = new Board(this);
    console = new Console(this);

    me->setGeometry(20, 20, 230, 300);
    board->setGeometry(230, 0, 300, 300);
    computer->setGeometry(800 - 50 - 200, 20, 230, 300);
    console->setGeometry(30, 320,500, 260);

    //有关按钮
    shake = new QPushButton(this);
    finish =new QPushButton(this);

    shake->setGeometry(590,380,150,60);
    shake->setText("SHAKE");
    finish->setGeometry(590,480,150,60);
    finish->setText("FINISH");

    finishClicked=0;

    connect(shake,&QPushButton::released,board,&Board::shake);
    connect(shake,&QPushButton::released,this,&BoggleWindow::clearConsole);
    connect(finish,&QPushButton::released,this,&BoggleWindow::findAll);
     connect(finish,&QPushButton::released,this,&BoggleWindow::clickFinish);
    connect(shake,&QPushButton::released,me,&WordListWidget::playAgain);
    connect(shake,&QPushButton::released,computer,&WordListWidget::playAgain);

    //有关 点击游戏
    connect(board,&Board::cubeClick,this,&BoggleWindow::clickSearch);
    nextx=-1;
    nexty=-1;

    //有关新词写入
    connect(console,&Console::newLineWritten,this,&BoggleWindow::receiveNewWord);

   //有关读入文件
    QFile qFile(":/res/EnglishWords.txt");
    if (!qFile.open(QIODevice::ReadOnly)) {
        throw new std::runtime_error("Resource file not found!");
    }
    lex=new Lexicon(qFile);


    /*
    for (std::string s: lex) {
        std::cout << s << std::endl;
    }
    */
    console->write("Welcome to the game Boggle!\n");
}


BoggleWindow::~BoggleWindow()
{
}

//在点击FINISH按钮后或者按了回车 finishClicked值变为1 阻止玩家继续输入和点击Cube
//finishClicked的值在再次点击shake后在clearConsole函数里变为0
void BoggleWindow::clickFinish()
{
    finishClicked=1;
}
//玩家输入
void BoggleWindow::receiveNewWord(const QString newline)
{
    //若是回车 则电脑查找
    if(newline=="")
    {
        finishClicked=1;
        findAll();
        return;
    }

    if(finishClicked)
    {
        console->write("GAME OVER!!!\n");
        return;
    }

    //判断输入单词是否合法
    //把所有按钮背景色改成白的
    for(int i=0;i<board->size;i++)
    {
        for(int j=0;j<board->size;j++)
        {

           board->cubes[board->index(i,j)]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");


        }
    }


    QString string=newline;
    string=string.toLower();   //都变成小写的

    //是否超过四个字母
    if(string.size()<4) return;

    //是否已包含
    if(me->contains(string))
    {
        console->write("YOU HAVE ALREADY FOUND THIS WORD!!!\n");
        return;
    }
    if(me->contains(string) || computer->contains((string))) return;

    //是否是英语单词
    //QString->string
    std::string newWord;
    newWord = string.toStdString() ;
    if(!lex->contains(newWord))
    {
        console->write("NO VALID WORD!!!\n");
        return;
    }

    //判断board里有没有这个单词
    bool flag =0;
    for(int i=0;i<board->size;i++)
    {
        for(int j=0;j<board->size;j++)
        {
            QString tmp=board->cubes[board->index(i,j)]->text();
            tmp=tmp.toLower();
            if(tmp == string.at(0))
            {

                //visited标记已经访问的 元素
                int visited[5][5];
                for(int i=0;i<5;i++)
                {
                    for(int j=0;j<5;j++)
                    {
                        visited[i][j]=0;
                    }
                }

                visited[i][j]=1;

                //Q查找特殊
                if(string.at(0)=='q')
                {
                    //说明第2个字母是u,那么直接检查第三个字母
                    flag=search(string,2,board,i,j,visited);
                }
                else
                {
                    flag=search(string,1,board,i,j,visited);
                }
                visited[i][j]=0;


                if(flag)
                {
                  board->cubes[board->index(i,j)]->setStyleSheet("background-color: rgb(246,186,191); border-radius: 15px; border: 2px solid");
                  me->addWord(string);
                  me->addScore(string.length()-3);
                  console->write("CONGRATULATINS!!!YOU FOUND A WORD!!!\n");
                  break;
                }
            }
        }

        if(flag) break;

    }
    if(!flag)
    {
        console->write("YOUR WORD CAN NOT BE CONNECTED IN THE BOARD!!!\n");
    }





}

//玩家查找
bool BoggleWindow::search(QString newword, int checknow, Board *board,int x,int y,int visited[][5])
{
    //终止条件
    if(checknow==newword.size()) return 1;

    int next[8][2]={{0,-1},{0,1},{-1,0},{1,0},{-1,-1},{1,-1},{-1,1},{1,1}};  //左右上下 左上 左下 右上 右下
    bool flag=0;

    for(int i=0;i<8;i++)
    {

        int tmpx=x+next[i][0];
        int tmpy=y+next[i][1];

        if(tmpx<0 || tmpx>4 ||tmpy<0 || tmpy>4) continue;
        if(visited[tmpx][tmpy])  continue; //防止重复走
        QString tmp=board->cubes[board->index(tmpx,tmpy)]->text();
        tmp=tmp.toLower();
        if(tmp ==newword.at(checknow))
        {
            visited[tmpx][tmpy]=1;

            if(tmp=='q')
            {
                flag=search(newword,checknow+2,board,tmpx,tmpy,visited);
            }
            else{
                flag=search(newword,checknow+1,board,tmpx,tmpy,visited);
            }
            visited[tmpx][tmpy]=0;
            if(flag==1)
            {
                board->cubes[board->index(tmpx,tmpy)]->setStyleSheet("background-color:rgb(246,186,191); border-radius: 15px; border: 2px solid");
                return flag;
            }

        }


    }

    return flag;

}

//电脑查找
void BoggleWindow::findAll()
{

    for(int i=0;i<board->size;i++)
    {
        for(int j=0;j<board->size;j++)
        {
            QString check;

            QString tmp=board->cubes[board->index(i,j)]->text();
            tmp=tmp.toLower();

            if(tmp=='q')//如果是q直接在现有单词里添加qu
            {
                 check.append(tmp);
                 check.append('u');
            }
            else {
                check.append(tmp);
            }
            bool flag=0;

            int visited[5][5];

            for(int i=0;i<5;i++)
            {
                for(int j=0;j<5;j++)
                {
                    visited[i][j]=0;
                }
            }
            visited[i][j]=1;
            flag=searchAll(i,j,check,visited,board);
            visited[i][j]=0;

        }
    }

    computer->drawWord();

    if(me->getScore() <computer->getScore())
    {
        console->write("GAME OVER!!!\nYOU WERE BEATEN BY THE COMPUTER!!!\n");
    }
    else if(me->getScore() == computer->getScore())
    {
        console->write("DRAWN GAME!!!\n");
    }
    else
    {
        console->write("GAME OVER!!!\nCONGRATULATINS!!!YOU BEAT THE COMPUTER\n");
    }



}

bool BoggleWindow::searchAll(int x, int y, QString nowString, int visited[][5], Board *board)
{
    //终止条件

    if(nowString.length()>=2)
    {

         std::string newWord;
        newWord = nowString.toStdString() ;
        if(!lex->containsPrefix(newWord))
        {
            if(nowString.at(nowString.length()-1)=='u'  && nowString.at(nowString.length()-2)=='q')
            {
                nowString=nowString.left(nowString.length()-2);
            }
            else
            {
                nowString=nowString.left(nowString.length()-1);
            }
            return 0;
        }
    }
    if(nowString.length()>=4 && !me->contains(nowString) &&!computer->contains(nowString))
    {

        std::string newWord;
        newWord = nowString.toStdString() ;
        if(lex->contains(newWord))
        {
            computer->addScore(nowString.length()-3);
            computer->addWordDontDraw(nowString);

        }
    }

    int next[8][2]={{0,-1},{0,1},{-1,0},{1,0},{-1,-1},{1,-1},{-1,1},{1,1}};  //左右上下 左上 左下 右上 右下
    bool flag=0;

    for(int i=0;i<8;i++)
    {

        int tmpx=x+next[i][0];
        int tmpy=y+next[i][1];


        if(tmpx<0 || tmpx>4 ||tmpy<0 || tmpy>4) continue;
        if(visited[tmpx][tmpy]) continue;
        QString tmp=board->cubes[board->index(tmpx,tmpy)]->text();
        tmp=tmp.toLower();

        if(tmp=='q')
        {
             nowString.append(tmp);
              nowString.append('u');
        }
        else {
             nowString.append(tmp);
        }

        visited[tmpx][tmpy]=1;

        flag=searchAll(tmpx,tmpy,nowString,visited,board);
        visited[tmpx][tmpy]=0;
        if(nowString.at(nowString.length()-1)=='u'  && nowString.at(nowString.length()-2)=='q')
        {
            nowString=nowString.left(nowString.length()-2);
        }
        else
        {
            nowString=nowString.left(nowString.length()-1);
        }


    }

    return flag;


}

void BoggleWindow::clearConsole()
{
    finishClicked=0;
    console->cclear();
    console->write("NEW GAME!!!\n");
}

void BoggleWindow::clickSearch(int x,int y)
{
    //如果电脑已经找过再次点击就无效
    if(finishClicked)
    {
        console->write("GAME OVER!!!\n");
        for (int i = 0; i <5; ++i) {
            for (int j = 0; j < 5; ++j)
            {
                board->cubes[board->index(i,j)]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");

            }
        }
        return;
    }
    QString tmp=board->cubes[board->index(x,y)]->text();
    tmp=tmp.toLower();

    //特判q
    if(tmp=='q')
    {
        clickCheck.append(tmp);
        clickCheck.append('u');
    }
    else {
        clickCheck.append(tmp);
    }


    /*
     * 无需判断改方块是否点击过
     * 因为在void Board::mouseReleaseEvent（）里已经有判断
     * 只有白色方块 也就是没被点击过的方块被电击才会发射信号
    */


    int next[8][2]={{0,-1},{0,1},{-1,0},{1,0},{-1,-1},{1,-1},{-1,1},{1,1}};  //左右上下 左上 左下 右上 右下

    //判断新点击的方块是否在刚刚点击的方块边上
    if(nextx!=-1 && nexty!=-1)
    {
        int tmpx,tmpy;
        int flag=0;
        for(int i=0;i<8;i++)
        {
            tmpx=nextx+next[i][0];
            tmpy=nexty+next[i][1];
            if(tmpx==x &&tmpy ==y)
            {
                flag=1;
                break;
            }
        }

        if(!flag)//说明不在边上
        {

            for (int i = 0; i <5; ++i) {
                for (int j = 0; j < 5; ++j)
                {
                    board->cubes[board->index(i,j)]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");

                }
            }
            nextx=-1;
            nexty=-1;
            clickCheck.clear();
            console->write("THIS CUBE IS NOT NEXT TO THE LAST CUBE!!!\n");
            return;

        }

    }
    else {
        nextx=x;
        nexty=y;
    }

    //判断是否有该前缀
    if(clickCheck.length()>=2)
    {

         std::string newWord;
         newWord = clickCheck.toStdString() ;

        //如果不含该前缀  全部变白
        if(!lex->containsPrefix(newWord))
        {

            for (int i = 0; i <5; ++i) {
                for (int j = 0; j < 5; ++j)
                {
                    board->cubes[board->index(i,j)]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");

                }
            }
            console->write("NO VALID WORD!!!\n");
            clickCheck.clear();
            nextx=-1;
            nexty=-1;
            return;

        }
        nextx=x;
        nexty=y;
    }

    //判断是否自己已经找过  如果自己已经找过不会终止 可以接着找更长的
    if(me->contains(clickCheck))
    {
        console->write("YOU HAVE ALREADY FOUND THIS WORD!!!\nYOU CAN GO ON TO FIND A LONGER ONE!!!\n");
    }

    //判断是不是单词
    if(clickCheck.length()>=4 && !me->contains(clickCheck))//&&!computer->contains(clickCheck)
    {

        std::string newWord;
        newWord = clickCheck.toStdString() ;
        if(lex->contains(newWord))
        {

            me->addScore(clickCheck.length()-3);
            me->addWord(clickCheck);


            //等待100ms 让有效单词高亮
            QEventLoop eventloop;
            QTimer::singleShot(100, &eventloop, SLOT(quit()));
            eventloop.exec();
            for (int i = 0; i <5; ++i) {
                for (int j = 0; j < 5; ++j)
                {
                    board->cubes[board->index(i,j)]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");

                }
            }
            nextx=-1;
            nexty=-1;
            clickCheck.clear();
            console->write("CONGRATULATINS!!!YOU FOUND A WORD!!!\n");

        }
    }


}
