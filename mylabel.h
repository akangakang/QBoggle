#ifndef MYLABEL_H
#define MYLABEL_H
#include <QLabel>

class mylabel:public QLabel
{
    Q_OBJECT
public:
   mylabel();

signals:


protected:
     void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // MYLABEL_H
