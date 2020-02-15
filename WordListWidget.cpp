#include "WordListWidget.h"
#include "WordTable.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTableView>

WordListWidget::WordListWidget(QWidget *parent, QString label) : QWidget(parent)
{
    reset();

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *headLayout = new QHBoxLayout();

    QLabel *nameLabel = new QLabel(this);
    scoreLabel = new QLabel(this);
    QFont font = nameLabel->font();
    font.setPointSize(20);
    nameLabel->setFont(font);
    scoreLabel->setFont(font);
    nameLabel->setText(label);
    scoreLabel->setText(QString::number(score));

    headLayout->addWidget(nameLabel, 0, Qt::AlignmentFlag::AlignLeft);
    headLayout->addWidget(scoreLabel, 0, Qt::AlignmentFlag::AlignRight);
    layout->addLayout(headLayout);

    QFrame *hline = new QFrame();
    hline->setFrameShape(QFrame::HLine);
    hline->setFrameShadow(QFrame::Sunken);
    layout->addWidget(hline);

    wordTable = new WordTable();
    layout->addWidget(wordTable);

    setLayout(layout);
}

void WordListWidget::addScore(int a)
{
    score += a;
    scoreLabel->setText(QString::number(score));


}
void WordListWidget::addWordDontDraw(QString word)
{
    this->words.append(word);
}
void WordListWidget::addWord(QString word)
{
    this->words.append(word);
    this->wordTable->addWord(word);
}
void WordListWidget::drawWord(){
    this->wordTable->drawAllWord(words);
}
int WordListWidget::getScore()
{
    return score;
}
void WordListWidget::reset()
{
    score = 0;
    words.clear();


}

bool WordListWidget::contains(QString word)
{
    return this->words.contains(word);
}

void WordListWidget::playAgain()
{
    score = 0;
    wordTable->clearall();
    words.clear();
    scoreLabel->setText(QString::number(score));


}
