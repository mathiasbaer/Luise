#include "GraphicsContainer.h"

void GraphicsContainer::create() {
	buffer.allocate(ofGetWidth(),ofGetHeight());
	buffer.begin();
	ofClear(0, 0, 0, 0);
	buffer.end();
}

void GraphicsContainer::clear() {
	buffer.begin();
	ofClear(0, 0, 0, 0);
	buffer.end();
}

void GraphicsContainer::setImage( ofImage _image ) {
	image = _image;
	steps = 0;
	buffer.begin();
	ofClear(0, 0, 0, 0);
	buffer.end();
	position = ofVec2f( ofGetWidth()/2 - image.getWidth()/2, ofGetHeight()/2 - image.getHeight()/2);
}

void GraphicsContainer::update() {
	position += ofVec2f( ofRandom(-10,10),ofRandom(-10,10) );
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