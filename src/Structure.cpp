#include "Structure.h"

void Structure::create( float _posX, float _posY, std::vector<Fragment*> _fs ) {
    
    mHasTrackingPoint = true;
    mTrackingPointPosition.set(_posX, _posY);
    
	children = _fs;
	leader.attach(_posX, _posY);
	setBindings();
    
}

void Structure::setBindings() {
	
	int vecLength = children.size();
	for ( int i=0; i<vecLength; i++ ) {
		
		Fragment* child = children[i];
		
		if (i > 0) {
			// pass fragment id because passing a fragment object does not work
            child->setTarget( children[i-1] );
		} else {
			// same as fragment. pass structure id and true for follow leader
			child->setTarget( &leader );
		}
		child->setPropertiesWithIndex( i );
    }
}

void Structure::addChildren( std::vector<Fragment*> _fs ) {
	
	int ln = _fs.size();
	for (int i=0; i<ln; i++) {
		children.push_back(_fs[i]);
	}
	setBindings();
}


void Structure::draw() {
	
	ofPushStyle();
	ofEnableAlphaBlending();
	
    leader.draw();
	
	int ln = children.size();
	
	for (int i=0; i<ln-1; i++) {
		Fragment* f0 = children[i];
		
		for (int j=i+1; j<ln; j++) {
			Fragment* f1 = children[j];
			
			float dist = f0->position.distance(f1->position);
			
			if (dist<100 && f0->opacityActive < 0.8) {
				float op = (1-dist/100);
				if (op>f0->opacityActive) op = f0->opacityActive;
				ofSetColor(f0->fillColor, op*100);
				ofLine(f0->position.x,f0->position.y,f1->position.x,f1->position.y); 
			}
		}
	}
	ofDisableAlphaBlending();
	ofPopStyle();
}

void Structure::update() {
    
    if(!mHasTrackingPoint) {
        float timeDiff = ofGetElapsedTimef() - mlostTime;
        if(timeDiff > 3) { mDoDelete = true; }
    } else {
        leader.update(mTrackingPointPosition.x, mTrackingPointPosition.y);
    }
}

void Structure::update(float _x, float _y) {
	leader.update(_x, _y);
}

void Structure::setTrackingPoint(bool _hasTrackingPoint) {
    //Losttime setzen wenn kein Trackingpoint mehr
    if(!_hasTrackingPoint) { 
        if(mlostTime == 0) { mlostTime = ofGetElapsedTimef(); }
    }
    else { mlostTime = 0; }
    
    mHasTrackingPoint = _hasTrackingPoint;
}

void Structure::setTrackingPoint(bool _hasTrackingPoint, ofVec2f _pos) {
    //Losttime setzen wenn kein Trackingpoint mehr
    if(!_hasTrackingPoint) { 
        if(mlostTime == 0) { mlostTime = ofGetElapsedTimef(); }
    }
    else { mlostTime = 0; }
    
    mHasTrackingPoint = _hasTrackingPoint;
    mTrackingPointPosition = _pos;
}

void Structure::destroy() {
    
    for ( int i=0; i<children.size(); i++ ) {
        children[i]->clearTarget();
    } 
    
}
