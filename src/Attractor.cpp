#include "Attractor.h"

void Attractor::create() {
	position.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
}

void Attractor::update() {
	
	rotation += ofRandom(-PI/4, PI/4);
	vel.set( cos(rotation)*speed, sin(rotation)*speed );
	position += vel;
	checkBoundries();
}

void Attractor::draw() {
	
	ofPushMatrix();
	glTranslated(position.x, position.y, 0);
	ofSetColor(0,255,0);
	ofCircle(0, 0, 20);
	ofPopMatrix();

}

void Attractor::checkBoundries() {
    
	if ( position.x > ofGetWidth())  position.x = 0;
	if ( position.x < 0 )			 position.x = ofGetWidth();
	if ( position.y > ofGetHeight()) position.y = 0;
	if ( position.y < 0 )            position.y = ofGetHeight();
}