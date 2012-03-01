#ifndef schirn_foyer_trackingpoint_h
#define schirn_foyer_trackingpoint_h

#include "ofMain.h"
#include "displayObject.h"

class TrackingPoint: public DisplayObject {
    
public:
    
    TrackingPoint(ofVec2f _pos): DisplayObject() {
        hasStructure = false;
        position = _pos;
    }
	
	void    update(ofVec2f _pos);
    float   checkDist(ofVec2f _blobPos);
    void    draw();
    
    bool    hasStructure;
	
};


#endif
