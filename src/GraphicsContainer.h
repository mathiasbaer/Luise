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
		
		blockTime = 1;
		elapsedTime = 0;
    }
	
	void create();
	void init(ofImage _image, float _x, float _y);
	void update();
	void draw();
	void setImage( ofImage _image );
	void clear();
	
	void setPosition( float x, float y );
    void setPosition( int x, int y );
	
	int steps;
	int maxSteps;
	
	float blockTime;
	float elapsedTime;
	
	bool canDraw;
	
	ofImage image;
	ofFbo buffer;
	
};


#endif