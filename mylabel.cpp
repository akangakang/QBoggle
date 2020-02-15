#include "mylabel.h"

mylabel::mylabel()
{

}

void mylabel::mouseReleaseEvent(QMouseEvent *ev)
{
     this->setStyleSheet("background-color:rgb(246,186,191); border-radius: 15px; border: 2px solid");

}
