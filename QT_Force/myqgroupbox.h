#ifndef MYQGROUPBOX_H
#define MYQGROUPBOX_H

#include <QGroupBox>
#include <QWidget>
#include <Qt>

class MyQGroupBox : public QGroupBox
{
    Q_OBJECT
public:
    MyQGroupBox(QWidget* parent = Q_NULLPTR);
    ~MyQGroupBox();

signals:
    void leaved();

protected:
    void leaveEvent(QEvent* event);
};

#endif // MYQGROUPBOX_H
