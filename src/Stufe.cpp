#include "Stufe.h"

void Stufe::init(ofVec2f _pos, int _w, int _h) {
    mPosition = _pos;
    mWidth = _w;
    mHeight = mTargetH = curHeight = _h;
    

}

void Stufe::setHeight(int _h) {
    mHeight = mTargetH = curHeight = _h;
}

void Stufe::move(int _h, float _speed) {
    mTargetH = _h;
}
void Stufe::update() {
    
    if(mTargetH == mHeight && curHeight == mHeight ) { return; }
    
    float newHeight = (mTargetH - curHeight)/(5*mSpeed);
    curHeight += newHeight; 
    
    if(ABS(curHeight - mTargetH) < 1) {
        curHeight = mTargetH;
        
        //wieder zurück
        if(curHeight != mHeight) { mTargetH = mHeight; }
    }
}
