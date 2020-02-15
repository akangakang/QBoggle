#ifndef BOARD_H
#define BOARD_H

#include "Cube.h"
#include "WordListWidget.h"
#include <QWidget>
#include<QPushButton>
#include <QMouseEvent>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr, int size = 5, const QString *cubeLetters = BIG_BOGGLE_CUBES);
    virtual ~Board();
    int size;
    Cube **cubes;


signals:
    void cubeClick(int x,int y);
public slots:
    void shake();

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
private:
    QPoint mouserPos;

public:

    QString *letters;
    inline int index(int i, int j) { return i * size + j; }
    static const QString STANDARD_CUBES[16];
    static const QString BIG_BOGGLE_CUBES[25];
};

#endif // BOARD_H
