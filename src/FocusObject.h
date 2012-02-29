#ifndef schirn_foyer_focusobject_h
#define schirn_foyer_focusobject_h

#include "ofMain.h"
#include "displayObject.h"
#include "LFO.h"

class FocusObject: public DisplayObject {
    
public:
    
    FocusObject(): DisplayObject() {
        isLeader = true;
    }
   
	void attach();
    void attach(float _x, float _y);
	void update(float _x, float _y);
	void draw();
	
};


#endif
