#include "TrackingPoint.h"

void TrackingPoint::create(float _x, float _y) {
	position.set(_x,_y);
}

void TrackingPoint::update(float _x, float _y) {
	position.set(_x,_y);
}