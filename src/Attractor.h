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
		numSatellites = 0;
		maxSatellites = 30;
    }
    
    void create();
    void update();
    void draw();
	void checkBoundries();
	void addSatellite(ofVec2f _v);
	void clearSatellites();
	bool hasEnough();
		
	int maxSatellites;
	float rotation;
	float speed;
	int numSatellites;
		
	std::vector<ofVec2f> satellites;
	
	ofVec2f position, vel;
};



#endif