#ifndef schirn_foyer_GraphicsContainer_h
#define schirn_foyer_GraphicsContainer_h

#include "ofMain.h"
#include "displayObject.h"

class GraphicsContainer: public DisplayObject {
    
public:
    
    GraphicsContainer(): DisplayObject() {
		steps = 0;
		maxSteps = 50;
		position = ofVec2f( ofGetWidth()/2, ofGetHeight()/2 );
		canDraw = false;
    }
	
	void create();
	void init();
	void update();
	void draw();
	void setImage( ofImage _image );
	void clear();
	
	void setPosition( float x, float y );
    void setPosition( int x, int y );
	
	int steps;
	int maxSteps;
	
	bool canDraw;
	
	ofImage image;
	ofFbo buffer;
	
};


#endif