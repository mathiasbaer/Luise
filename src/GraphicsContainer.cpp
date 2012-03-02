#include "GraphicsContainer.h"


void GraphicsContainer::create() {
	buffer.allocate(ofGetWidth(),ofGetHeight());
	init();
}

void GraphicsContainer::init() {
	steps = 0;
	clear();
	rotation = ofRandom(PI*2);
	speed = 5;
}

void GraphicsContainer::setPosition( float x, float y ) {
    pos( x - image.getWidth()/2 , y - image.getHeight()/2 );
}

void GraphicsContainer::setPosition( int x, int y ) {
    pos( (float) x - image.getWidth()/2 ,(float) y - image.getHeight()/2 );
}

void GraphicsContainer::clear() {
	buffer.begin();
	ofClear(0, 0, 0, 0);
	buffer.end();
}

void GraphicsContainer::setImage( ofImage _image ) {
	image = _image;
	init();
}

void GraphicsContainer::update() {
	rotation += ofRandom(-PI/10,PI/10);
	position += ofVec2f( cos(rotation)*speed,
						 sin(rotation)*speed );
	speed *= 0.9;
}

void GraphicsContainer::draw() {
	
	/*
	 * fade out buffer image slowly. semi transparent clear not working
	 */
	
	buffer.begin();
	//ofEnableAlphaBlending();
	//ofSetColor(0, 0, 0, 100);
	//ofRect(0, 0, ofGetWidth()-300, ofGetHeight());
	//ofClear(0, 0, 0, 127);
	//ofDisableAlphaBlending();
	buffer.end();
	
	
	// add image to buffer
	if (steps < maxSteps) {
		buffer.begin();
			//ofEnableAlphaBlending();
			ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		
				//ofSetColor(0, 0, 0, 100);
				//ofRect(0, 0, ofGetWidth()-300, ofGetHeight());
				image.draw(position.x, position.y);
		
			//ofDisableAlphaBlending();
			ofDisableBlendMode();
		buffer.end();
	}
		
	
	// draw final buffer image
	ofEnableBlendMode(OF_BLENDMODE_SCREEN);
	//ofEnableAlphaBlending();
	
		buffer.draw(0,0,ofGetWidth(),ofGetHeight());
	
	//ofDisableAlphaBlending();
	ofDisableBlendMode();
	
	steps++;
}