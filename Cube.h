#ifndef CUBE_H
#define CUBE_H

#include <QLabel>
#include <QWidget>


class Cube : public QWidget
{
    Q_OBJECT
public:
    explicit Cube(QWidget *parent = nullptr);
    void setLetter(QString l);
    QString text();
    bool contains(int x,int y);

signals:

public slots:

private:
    QLabel *label;
};

#endif // CUBE_H
