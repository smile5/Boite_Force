#include "myqpushbutton.h"

MyQPushButton::MyQPushButton(QWidget* parent)
   : QPushButton(parent) {

}

MyQPushButton::~MyQPushButton() {}


void MyQPushButton::leaveEvent(QEvent* event) {
    emit leaved();
}
