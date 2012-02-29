#ifndef schirn_foyer_trackingpoint_h
#define schirn_foyer_trackingpoint_h

#include "ofMain.h"
#include "displayObject.h"

class TrackingPoint: public DisplayObject {
    
public:
    
    TrackingPoint(): DisplayObject() {
        
    }
	
	void create();
	void update(float _x, float _y);
	
};


#endif
