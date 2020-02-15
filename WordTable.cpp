#include "WordTable.h"
#include <QHeaderView>
#include <QScrollBar>

WordTable::WordTable(QWidget *parent, int maxCol) : QTableWidget(parent)
{
    this->maxCol = maxCol;
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setShowGrid(false);
    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();
    this->horizontalScrollBar()->hide();
    this->verticalScrollBar()->hide();
    reset();
}

void WordTable::addWord(QString word)
{
    if (col == maxCol) {
        row++;
        this->model()->insertRow(row);
        col = 0;
    }
    this->setItem(row, col, new QTableWidgetItem(word));
    col++;
    this->resizeColumnsToContents();
    this->resizeRowsToContents();
}

void WordTable::drawAllWord(QList<QString> words)
{
    for(int i=0;i<words.length();i++)
    {
        if (col == maxCol) {
            row++;
            this->model()->insertRow(row);
            col = 0;
        }
        this->setItem(row, col, new QTableWidgetItem(words.at(i)));
        col++;

    }

    //这两个函数特别慢
    this->resizeColumnsToContents();
    this->resizeRowsToContents();

}

void WordTable::reset()
{
    row = 0;
    col = 0;
    this->setColumnCount(maxCol);
    this->setRowCount(1);
}

void WordTable::clearall()
{
   for(int i=0;i<=maxCol;i++)
   {
       this->removeColumn(i);
   }
   for(int i=0;i<=maxCol;i++)
   {
       this->removeColumn(i);
   }
   row = 0;
   col = 0;
   this->setColumnCount(maxCol);
   this->setRowCount(1);
}
