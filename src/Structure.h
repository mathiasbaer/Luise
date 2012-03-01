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
        
    }
	
    // seperate create and build functions if existing
	// structure needs to be rebuild or should have no children
	
	void create( float _posX, float _posY, std::vector<Fragment*> _fs );
	void addChildren( std::vector<Fragment*> _fs );
	void destroy();
	//void update(float _x, float _y);
    void update(std::vector<TrackingPoint> * _tpList);
	void draw();
    
    FocusObject leader;
	std::vector<Fragment*> children;
     
	
private:
	void setBindings();
};



#endif

