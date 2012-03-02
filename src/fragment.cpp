#include "fragment.h"


void Fragment::create(float _x, float _y) {
    
    fillColor.set(200,230,60);
    
    iRotation.setRange(PI/5);
    iRotation.baseValue = -PI/2;
    iRotation.speed = 0.01;
    
    iLength.setRange(100.0);
    iLength.minValue = 20.0;
    iLength.speed = 0.02;
	
	iOpacity.setRange(3);
	iOpacity.minValue = -2;
	iOpacity.speed = 0.02;
	iOpacity.radians = ofRandom(PI*2);
    
    // for free movement
    speed = 0.5;
    
    position.set(_x, _y);
    
}

int Fragment::findAttractor( std::vector<Attractor> _atts ) {
	
	int idx = -1;
	
	for (int i=0; i<_atts.size(); i++) {
		Attractor att = _atts[i];
		float dst = position.distance(att.position);
		if (dst < 100) {
			pull = att.position - position;
			pull /= 40;
			pull.limit(3);
			idx = i;
			break;
		}
	}
	return idx;
}

void Fragment::update() {
	
	lastPosition1.set(lastPosition0);
	lastPosition0.set(position);
   
    if( hasTarget || opacityActive > 0 ) {
		
		iRotation.update();
        iLength.update();
	}
	
	iOpacity.update();
	opacityBase = iOpacity.value;
	if (opacityBase < 0) opacityBase = 0;
    opacityBase *= 255;
	
    // is in structure
    if( hasTarget ) {
		
		//cout << target->po << endl;
		
        // current fragment
        ofVec2f v0 = position;
        // following this fragment
		ofVec2f v1 = target->position;
        
        
        // get desired position in structure => v1
        rotation = iRotation.value + target->rotation;
        ofVec2f offset;
		offset.set( cos(rotation), sin(rotation) );
        offset *= offsetToTarget;
        v1 += offset;
        
        
        desired = v1 - v0;
        
        float d = desired.length();
        targetDistance = d;
        
        if (d>0) {
			
			/*
            desired.normalize();
            if (d < 50.0f) desired *= 2*(d/50.0f);
            else desired *= 2;
            
            force = desired - vel;
            force *= 0.05;
            
            acc += force;
            acc.limit( 2 );
			*/
			
			ofVec2f v;
			v.set(1,1);
			v /= 5000;
			acc += v;
            acc.limit( 0.05 );
			desired *= acc;
			
			//desired *= 0.05;

        }
        else {
            acc.set(0, 0);
        }
        
        //vel += acc;
		position += desired;
		//position += desired;
        
        //acc *= 0.2;
		
        
        // set force to move fragment up if target is cleared next frame
        force.set(0,-1);
		//force.normalize();
		force *= 1;
		force += desired;
        //force += vel;
        //force.limit( 2 );
		
		// set position in structure as desired for next fragment
        desired = v1;
    }
    // free movement
    else {
		
		rotation = atan2( vel.y, vel.x ) + ofRandom(-PI/10, PI/10);
		
        vel.set(cos(rotation) * speed, sin(rotation) * speed);
        if (opacityActive > 0) { vel += force; }
		if (pull.length() > 0.1) {
			vel += pull;
			//opacity = ofMap(pull.length(), 0, 100, opacityBase, 255, false);
			opacity = 255;
		}
		else {
			pull *= 0;
			opacity = opacityBase;
		}
        position += vel;
        force *= 0.99f;
		if (force.length()<0.1) { force *= 0; };
		pull *= 0.8;
		
    }
    
    // reset rotation for next fragment
    rotation -= iRotation.baseValue;
	
    length = iLength.value;
    
    checkBoundries();
}

void Fragment::setPropertiesWithIndex( int _i ) {
    childIndex = _i;
    iRotation.radians = _i * PI/20;
    iLength.radians = _i * PI/20;
}

void Fragment::draw() {
	ofPushStyle();
	ofEnableAlphaBlending();
    glPushMatrix();
    //glTranslated(position.x, position.y,0);
    
    if (hasTarget) {
        if (opacityActive < 1) opacityActive += 0.01;
        if (targetDistance > 150 && opacityActive < 0.9) opacityActive = 0;
    }
    else {
        if (opacityActive > 0.01) opacityActive *= 0.99;
        else opacityActive = 0;
    }
    if (opacityActive > 0) {
        
        ofColor c = fillColor;

//        ofVec2f d = target.position - position;
        
        /*
         if ( d.mag() < 100 ) {
         float op = 1 - d.mag()/100;
         if (op>opacityActive) op = opacityActive;
         stroke(fillColor, op*250);
         strokeWeight(30);
         line(0,0, target.position.x - position.x, target.position.y - position.y);
         }
         */
        
        
        /*
         fill(fillColor);
         rect(-abs(d.x)/2,-length,abs(d.x),length);
         */
		
		glPushMatrix();
		glTranslated(position.x, position.y, 0);
		
        ofSetColor(c, opacityActive*255/4);
        ofSetLineWidth(4);
        ofLine(0,-length*3, 0,length/2);
        
        ofSetColor(c, opacityActive*255/1.5);
        ofSetLineWidth(2);
        ofLine(0,-length/2, 0,length/2);
		
		glPopMatrix();
    }
    
//    noStroke();
    
    if(hasTarget) {
        ofSetColor(fillColor,opacityActive);
	
		//tail
		
		ofSetLineWidth(lastPosition0.distance(lastPosition1));
		ofLine(lastPosition0.x, lastPosition0.y,
			   lastPosition1.x, lastPosition1.y);
		
		ofSetLineWidth(position.distance(lastPosition0));
		ofLine(position.x, position.y,
			   lastPosition0.x, lastPosition0.y);
	} else {
        ofSetColor(fillColor,opacity/2);
	}
	
	    
    ofEllipse(position.x,position.y,5,5);
    
    ofSetColor(fillColor,opacity);
    ofEllipse(position.x,position.y,2,2);
	
    glPopMatrix();
	ofDisableAlphaBlending();
	ofPopStyle();
}

