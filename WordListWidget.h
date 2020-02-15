#ifndef WORDLISTWIDGET_H
#define WORDLISTWIDGET_H

#include "WordTable.h"
#include <QLabel>
#include <QWidget>

class WordListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WordListWidget(QWidget *parent = nullptr, QString label = "");
    void addScore(int s);
    void addWord(QString word);
    void addWordDontDraw(QString word);

    bool contains(QString word);
    void reset();
    void drawWord();
    int getScore();
signals:

public slots:
    void playAgain();


private:
    QString label;
    QLabel *scoreLabel;
    QList<QString> words;
    int score;
    WordTable *wordTable;
};

#endif // WORDLISTWIDGET_H
