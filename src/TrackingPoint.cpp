#include "TrackingPoint.h"

void TrackingPoint::update(ofVec2f _pos) {
	position = _pos;
}

float TrackingPoint::checkDist(ofVec2f _blobPos) {
    return position.distance(_blobPos);
}

void TrackingPoint::draw() {
    ofSetColor(0, 0, 255);
    ofCircle(position.x, position.y, 10);
}