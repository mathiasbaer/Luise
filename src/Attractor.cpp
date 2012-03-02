#include "Attractor.h"

void Attractor::create() {
	position.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
}

void Attractor::update() {
	
	rotation += ofRandom(-PI/10, PI/10);
	vel.set( cos(rotation)*speed, sin(rotation)*speed );
	position += vel;
	checkBoundries();
	
	hasEnough = false;
	
	if (satellites > maxSatellites){
		cout << satellites << endl;
		hasEnough = true;
	}
	satellites = 0;
}

void Attractor::draw() {
	
	ofPushMatrix();
	glTranslated(position.x, position.y, 0);
	ofSetColor(0,255,0);
	ofCircle(0, 0, 20);
	ofPopMatrix();
}

void Attractor::checkBoundries() {
    
	if ( position.x > ofGetWidth() + 50 )  rotation = PI;
	if ( position.x < -50 )				   rotation = 0;
	if ( position.y > ofGetHeight() + 50)  rotation = -PI/2;
	if ( position.y < -50 )				   rotation = PI/2;
}