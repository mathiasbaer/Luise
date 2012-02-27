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
        
        mColor.r        = 255;
        mColor.g        = 255;  
        mColor.b        = 255;  
        mColor.a        = 255;  
        
    };

    void checkBoundries();
    void setTarget( DisplayObject t );
    void clearTarget();
   // void setStructure( Structure s );
    void setPosition( float x, float y );
    void setPosition( int x, int y );
    
    // for graphic in structure mode
    float       length; 
    float       rotation;
    float       targetDistance;
    float       speed;
    float       friction;
    float       easing;
    float       offsetToTarget;
    ofVec2f     position, desired, vel, acc, force;
    
    bool        hasTarget;
    bool        isLeader;
    
    // position in structure
    int childIndex;
    
    //Structure structure;
    //DisplayObject target;
    
    ofColor     mColor;
    
    
private: 
    
    void pos ( float x, float y );
    
};

#endif
