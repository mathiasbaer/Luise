#ifndef schirn_foyer_trackingpoint_h
#define schirn_foyer_trackingpoint_h

#include "ofMain.h"
#include "displayObject.h"

#define CAMWIDTH 320  
#define CAMHEIGHT 240 

class TrackingPoint: public DisplayObject {
    
public:
    
    TrackingPoint(ofVec2f _pos): DisplayObject() {
        hasStructure = false;
        position = _pos;
        
        mMapPos = ofVec2f(0,0);
    }
	
	void    update(ofVec2f _pos);
    float   checkDist(ofVec2f _blobPos);
    void    draw();
    
    bool    hasStructure;
    
    ofVec2f mMapPos;
	
};


#endif
