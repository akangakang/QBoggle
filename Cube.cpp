#include "Cube.h"
#include <QHBoxLayout>

Cube::Cube(QWidget *parent) : QWidget(parent)
{
    label = new QLabel();
    label->setText("");
    label->setAlignment(Qt::AlignCenter);
    QFont font = label->font();
    font.setPointSize(20);
    label->setFont(font);

    this->setFixedSize(75, 75);
    this->setStyleSheet("background-color: white; border-radius: 15px; border: 2px solid");

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);
    setLayout(layout);
}

void Cube::setLetter(QString l)
{
    label->setText(l);

}

QString Cube::text()
{
    return label->text();
}

bool Cube::contains(int x, int y)
{
    if(x>this->x()+11 && x<this->x()+64 && y>this->y()+11 && y<this->y()+64)
        return 1;
    else return 0;
}
