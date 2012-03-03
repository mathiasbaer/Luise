#include "displayObject.h"


void DisplayObject::checkBoundries() {
    
    if (!hasTarget) {
        int xEnd = mBoundary.x + mBoundary.width;
        int yEnd = mBoundary.y + mBoundary.height;
        
        if ( position.x > xEnd + 30 )               position.x = xEnd;
        if ( position.x < mBoundary.x - 30 )        position.x = mBoundary.x;
        if ( position.y > yEnd +30 )                position.y = yEnd;
        if ( position.y < mBoundary.y - 30 )        position.y = mBoundary.y;
    }
}


void DisplayObject::clearTarget() {
    
    //target = null;
    hasTarget = false;
    
}

void DisplayObject::setTarget( DisplayObject* _t ) {
	target = _t;
	hasTarget = true;
}

/*
void DisplayObject::setStructure( Structure s ) {

    this.structure = s;

}
*/

void DisplayObject::setBoundary(ofRectangle _b) {
    mBoundary = _b;
}

void DisplayObject::setPosition( float x, float y ) {
    pos( x,y );
}

void DisplayObject::setPosition( int x, int y ) {
    pos(float(x),float(y));
}


void DisplayObject::pos ( float x, float y ) {
    position.set(x, y);
    if (!hasTarget) { desired = position; }
}

