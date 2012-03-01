#ifndef schirn_foyer_structure_h
#define schirn_foyer_structure_h

#include "ofMain.h"
#include "displayObject.h"
#include "fragment.h"
#include "FocusObject.h"
#include "TrackingPoint.h"


class Structure {
    
public:
    
	Structure() {    
        mHasTrackingPoint = false;
        mTrackingPointPosition = ofVec2f(0,0);
        mDoDelete = false;
        mlostTime = 0;
    }
	
    // seperate create and build functions if existing
	// structure needs to be rebuild or should have no children
	
	void create( float _posX, float _posY, std::vector<Fragment*> _fs );
	void addChildren( std::vector<Fragment*> _fs );
	void destroy();
    void update();
	void update(float _x, float _y);
    void setTrackingPoint(bool _hasTrackingPoint); 
    void setTrackingPoint(bool _hasTrackingPoint, ofVec2f _pos);
	void draw();
    
    
    FocusObject leader;
	std::vector<Fragment*> children;
    bool mDoDelete;
    bool mHasTrackingPoint;
    ofVec2f mTrackingPointPosition;
    float mlostTime;
     
	
private:
	void setBindings();
};



#endif

