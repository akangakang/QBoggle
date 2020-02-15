#include "Console.h"
#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>

Console::Console(QWidget *parent) : QTextEdit(parent)
{
}

void Console::cclear()
{
    this->clear();
}

void Console::write(QString msg)
{
    this->append(msg);
}

void Console::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace)
    {

    }
    if (event->key() == Qt::Key_Delete)
    {

    }

    if (this->textCursor().hasSelection())
        return;

    //回车键检测单词
    if (event->key() == Qt::Key_Return) {


        QTextCursor cursor = this->textCursor();

       cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
        newLineWritten(lastLine);
    }
    QTextEdit::keyPressEvent(event);
}


