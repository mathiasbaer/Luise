#include "FocusObject.h"

void FocusObject::attach() {
    position.set(ofRandomWidth(), 0);
}

void FocusObject::attach(float _x, float _y) {
    position.set(_x,_y);
	desired.set(_x,_y);
}

void FocusObject::update(float _x, float _y) {

}

void FocusObject::draw() {

}