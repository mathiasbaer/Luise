#include "TrackingPoint.h"

void TrackingPoint::update(ofVec2f _pos) {
	position = _pos;
    
    //Mapping
    mMapPos.x = ofMap(position.x, 0, mCamWidth, 0, ofGetWidth());
    mMapPos.y = ofMap(position.y, 0, mCamHeight, 0, ofGetHeight());
    
}

float TrackingPoint::checkDist(ofVec2f _blobPos) {
    return position.distance(_blobPos);
}

void TrackingPoint::draw() {
    ofSetColor(0, 0, 255);
    ofCircle(position.x, position.y, 10);
}