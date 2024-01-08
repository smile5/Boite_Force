#include "myqgroupbox.h"

MyQGroupBox::MyQGroupBox(QWidget* parent)
   : QGroupBox(parent) {

}

MyQGroupBox::~MyQGroupBox() {}


void MyQGroupBox::leaveEvent(QEvent* event) {
    emit leaved();
}
