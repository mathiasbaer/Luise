#ifndef schirn_foyer_displayObject_h
#define schirn_foyer_displayObject_h

#include "ofMain.h"

class DisplayObject {
 
public:
    
    DisplayObject() {
        
        length          = 0;
        rotation        = 0;
        targetDistance  = 0;
        speed           = 3;
        friction        = 0.1;
        easing          = 0.1;
        offsetToTarget  = 10;
        
        hasTarget       = false;
        isLeader        = false;
        
        childIndex      = 0;
        
        fillColor.r        = 200;
        fillColor.g        = 230;  
        fillColor.b        = 60;  
        fillColor.a        = 255;  
        
        //Pointer am Anfang immer auf Null setzten!
        target              = NULL;
		position.set(0,0);
		desired.set(0,0);
		vel.set(0,0);
		acc.set(0,0);
		force.set(0,0);
		lastPosition0.set(0,0);
		lastPosition1.set(0,0);
        
    };    

    void checkBoundries();
    void clearTarget();
	void setTarget( DisplayObject* _t );
    void setPosition( float x, float y );
    void setPosition( int x, int y );
	void pos ( float x, float y );
    
    // for graphic in structure mode
    float           length; 
    float           rotation;
    float           targetDistance;
    float           speed;
    float           friction;
    float           easing;
    float           offsetToTarget;
    ofVec2f         position, desired, vel, acc, force, lastPosition0, lastPosition1;
	
	DisplayObject*	target;
    
    bool            hasTarget;
    bool            isLeader;
    
    // position in structure
    int             childIndex;
    
    //Structure structure;
    
    ofColor     fillColor;
    
};

#endif
