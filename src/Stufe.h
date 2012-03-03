#ifndef schirn_foyer_Stufen_h
#define schirn_foyer_Stufen_h

#include "ofMain.h"

class Stufe {
    
public:
    
    Stufe() {
        mPosition = ofVec2f(0,0);
        mWidth = 0;
        mHeight = mTargetH = 0;
		curHeight = 0.0f;
        mSpeed = 1.0f;
        
        mColor.r = 0;
        mColor.g = 0;
        mColor.b = 0;
        
    }
    
    void init(ofVec2f _pos, int _w, int _h);
    void move(int _h, float _speed);
    void update();
    
    ofVec2f mPosition;
    int     mWidth;
    int     mHeight;
    int     mTargetH;
    float   curHeight;
    float   mSpeed;
    ofColor mColor;
	
};


#endif

