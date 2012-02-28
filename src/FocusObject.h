#ifndef schirn_foyer_focusobject_h
#define schirn_foyer_focusobject_h

#include "ofMain.h"
#include "displayObject.h"
#include "LFO.h"

class FocusObject: public DisplayObject {
    
public:
    
    FocusObject(): DisplayObject() {
        isLeader = true;
		position.set(ofRandomWidth(), 0);
    }
   
	void create();
	void update();
	void draw();
	
};


#endif
