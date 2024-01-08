#ifndef MYQPUSHBUTTON_H
#define MYQPUSHBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <Qt>


class MyQPushButton: public QPushButton
{
    Q_OBJECT
public:
    MyQPushButton(QWidget* parent = Q_NULLPTR);
    ~MyQPushButton();

signals:
    void leaved();

protected:
    void leaveEvent(QEvent* event);
};

#endif // MYQPUSHBUTTON_H
