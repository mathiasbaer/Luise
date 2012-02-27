#include "displayObject.h"


void DisplayObject::checkBoundries() {
    
    if (!hasTarget) {
        if ( position.x > ofGetWidth() + 20 )  position.x = 0;
        if ( position.x < 0 - 20 )      position.x = ofGetWidth();
        if ( position.y > ofGetHeight() + 20 ) position.y = 0;
        if ( position.y < 0 - 20 )      position.y = ofGetHeight();
    }
    
}

void DisplayObject::setTarget( DisplayObject t ) {
    
   // target = t;
    hasTarget = true;

}

void DisplayObject::clearTarget() {
    
    //target = null;
    hasTarget = false;
    
}

//void DisplayObject::setStructure( Structure s ) {

    //this.structure = s;

//}

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

