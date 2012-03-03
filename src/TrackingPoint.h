#ifndef schirn_foyer_trackingpoint_h
#define schirn_foyer_trackingpoint_h

#include "ofMain.h"
#include "displayObject.h"

class TrackingPoint: public DisplayObject {
    
public:
    
    TrackingPoint(ofVec2f _pos, int _camWidth, int _camHeight): DisplayObject() {
        hasStructure = false;
        position = _pos;
        
        mMapPos = ofVec2f(0,0);
        
        mCamWidth = _camWidth;
        mCamHeight = _camHeight;
    }
	
	void    update(ofVec2f _pos);
    float   checkDist(ofVec2f _blobPos);
    void    draw();
    
    bool    hasStructure;
    
    ofVec2f mMapPos;
    
    int mCamWidth;
    int mCamHeight;
	
};


#endif
