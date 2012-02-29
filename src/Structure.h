#ifndef schirn_foyer_structure_h
#define schirn_foyer_structure_h

#include "ofMain.h"
#include "displayObject.h"
#include "fragment.h"
#include "FocusObject.h"


class Structure {
    
public:
    
	Structure() {    
		
    }
	
    // seperate create and build functions if existing
	// structure needs to be rebuild or should have no children
	void build( float _posX, float _posY, int _size, std::vector<Fragment> *_fs );
	void destroy();
    
	FocusObject leader;
	std::vector<Fragment> *children;
	
};



#endif

