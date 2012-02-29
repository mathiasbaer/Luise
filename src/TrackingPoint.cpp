#include "TrackingPoint.h"

void TrackingPoint::create(ofVec2f _pos) {
	position = _pos;
}

void TrackingPoint::update(ofVec2f _pos) {
	position = _pos;
}

float   TrackingPoint::checkDist(ofVec2f _blobPos) {
    return position.distance(_blobPos);
}