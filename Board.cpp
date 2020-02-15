#include "Board.h"
#include "Cube.h"
#include <ctime>
#include <QGridLayout>
#include <QDebug>

const QString Board::STANDARD_CUBES[16]  = {
        "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
        "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
        "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
        "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

const QString Board::BIG_BOGGLE_CUBES[25]  = {
        "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
        "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
        "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
        "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
        "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Board::Board(QWidget *parent, int size, const QString *cubeLetters) : QWidget(parent)
{
    this->size = size;
    this->cubes = new Cube*[size * size];
    this->letters = new QString[size * size];

    //把常量拷贝进letters
    for (int i = 0; i < size * size; ++i)
        this->letters[i] = cubeLetters[i];


    //画格子
    QGridLayout *layout = new QGridLayout();
    layout->setMargin(20);//外边距
    layout->setSpacing(10);//间距

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)] = new Cube(this);  //index给出从左到右从上到下第几个
            layout->addWidget(this->cubes[index(i, j)], i, j, Qt::AlignmentFlag::AlignCenter);//不同格子添加不同方块
        }
    }
    setLayout(layout);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            this->cubes[index(i, j)]->setLetter(this->letters[index(i, j)].at(0));
        }
    }
    // this->setStyleSheet("background-color:grey; border: 3px solid");






}

Board::~Board()
{
    if (cubes) delete[] cubes;
    if (letters) delete[] letters;
}


 // Shake Cubes
void Board::shake()
{




    //换cube
    //qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    qsrand(time(NULL));

    //调换letter
    for(int i=0;i<size*size;i++)
    {
        int k=qrand()%(size*size-i)+i;
        QString tmp;
        tmp=letters[i];
        letters[i]=letters[k];
        letters[k]=tmp;


    }


    //换朝上的字母
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cubes[index(i,j)]->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");
            int k=qrand()%5;
            this->cubes[index(i, j)]->setLetter(this->letters[index(i, j)].at(k));
        }
    }


    update();



}

//重写鼠标点击
void Board::mousePressEvent(QMouseEvent *ev)
{
    mouserPos = QPoint(ev->x(), ev->y());
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{

        for(int i=0;i<5;i++)
        {
            for(int j=0;j<5;j++)
            {
                if(cubes[(index(i,j))]->contains(mouserPos.x(),mouserPos.y()))
                {
                    if( cubes[index(i,j)]->styleSheet()=="background-color: white; border-radius: 15px; border: 2px solid")
                    {
                        cubes[index(i,j)]->setStyleSheet("background-color: rgb(246,186,191); border-radius: 15px; border: 2px solid");
                        emit cubeClick(i,j);
                    }

                }

            }
        }

}

