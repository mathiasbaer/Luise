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
		followsLeader	= false;
        
        childIndex      = 0;
        
        fillColor.r        = 255;
        fillColor.g        = 255;  
        fillColor.b        = 255;  
        fillColor.a        = 255;  
        
    };    

    void checkBoundries();
    virtual void setTarget() {};
    void clearTarget();
	//void setStructure( Structure s );
    void setPosition( float x, float y );
    void setPosition( int x, int y );
    
    // for graphic in structure mode
    float           length; 
    float           rotation;
    float           targetDistance;
    float           speed;
    float           friction;
    float           easing;
    float           offsetToTarget;
    ofVec2f         position, desired, vel, acc, force;
	
	DisplayObject*	target;
    
    bool            hasTarget;
    bool            isLeader;
	bool            followsLeader;
    
    // position in structure
    int             childIndex;
    
    //Structure structure;
    
    ofColor     fillColor;
    
    
private: 
    
    void pos ( float x, float y );
    
};

#endif
