#include "FocusObject.h"

void FocusObject::attach() {
    position.set(ofRandomWidth(), 0);
}

void FocusObject::attach(float _x, float _y) {
    position.set(_x,_y);
}

void FocusObject::update() {

}

void FocusObject::draw() {

}