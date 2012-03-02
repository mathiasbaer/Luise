#ifndef schirn_foyer_attractor_h
#define schirn_foyer_attractor_h

#include "ofMain.h"
#include "displayObject.h"
#include "LFO.h"

class Attractor: public DisplayObject {
    
public:
    
    Attractor(): DisplayObject() {
		
		vel.set(0,0);
		position.set(0,0);
        rotation = 0;
		speed = 10;
    }
    
    void create();
    void update();
    void draw();
	void checkBoundries();
	
	float rotation;
	float speed;
	
	ofVec2f position, vel;
};



#endif