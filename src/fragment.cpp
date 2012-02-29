#include "fragment.h"


void Fragment::create(float _x, float _y) {
    
    fillColor.set(200,230,60);
    
    iRotation.setRange(PI/20);
    iRotation.baseValue = -PI/2;
    iRotation.speed = 0.01;
    
    iLength.setRange(100.0);
    iLength.minValue = 20.0;
    iLength.speed = 0.05;
    
    // for free movement
    speed = 0.5;
    
    position.set(_x, _y);
    
}

void Fragment::update() {
    
    if( hasTarget || opacity > 0 ) {
		
		
		iRotation.update();
        iLength.update();
		
//        for ( int i=0; i<sizeof(lfos); i++ ) { lfos[i].update(); }
    }
 
    
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
			/*
			ofVec2f v(0.001, 0.001);
			acc += v;
            acc.limit( 0.5 );
			desired *= acc;
			*/
			desired *= 0.05;

        }
        else {
            acc.set(0, 0);
        }
        
        vel += acc;
		//position += vel;
        //position += desired;
		position += desired;
        
        acc *= 0.2;
        
        // set position in structure as desired for next fragment
        desired = v1;
        
        // set force to move fragment up if target is cleared next frame
        force.set(0,-1.5);
        force += vel;
        force.limit( 2 );
		
    }
    // free movement
    else {
        rotation = atan2( vel.y, vel.x ) + ofRandom(-PI/10, PI/10);
        vel.set(cos(rotation) * speed, sin(rotation) * speed);
        if (opacity > 0) { vel += force; }
        position += vel;
        force *= 0.99f;
        if (force.length()<0.5) { force *= 0; };
    }
    
    // reset rotation for next fragment
	cout << rotation << "---" ;
    rotation -= iRotation.baseValue;
	cout << rotation << endl;
	
    length = iLength.value;
    
    checkBoundries();
}

void Fragment::setPropertiesWithIndex( int _i ) {
    childIndex = _i;
    iRotation.radians = _i * PI/20;
    iLength.radians = _i * PI/20;
}

void Fragment::draw() {
    glPushMatrix();
    glTranslated(position.x, position.y,0);
    
    if (hasTarget) {
        if (opacity < 1) opacity += 0.01;
        if (targetDistance > 150 && opacity < 0.9) opacity = 0;
    }
    else {
        if (opacity > 0.01) opacity *= 0.99;
        else opacity = 0;
    }
    if (opacity > 0) {
        
        ofColor c = fillColor;

//        ofVec2f d = target.position - position;
        
        /*
         if ( d.mag() < 100 ) {
         float op = 1 - d.mag()/100;
         if (op>opacity) op = opacity;
         stroke(fillColor, op*250);
         strokeWeight(30);
         line(0,0, target.position.x - position.x, target.position.y - position.y);
         }
         */
        
        /*
        int con = 0;
        
        for (int i=0; i<structure.numChildren; i++) {
            Fragment f = structure.children[i];
            if (f!=this && con < 5) {
                PVector p = new PVector(f.position.x - position.x, f.position.y - position.y);
                float dist = PVector.sub(this.position,f.position).mag();
                if (dist<100 && opacity < 0.8) {
                    con++;
                    float op = (1-dist/100);
                    if (op>opacity) op = opacity;
                    stroke(fillColor, op*250);
                    line(0,0,p.x,p.y); 
                }
            }
        }
         */
        
        //glRotate produces a rotation of angle degrees around the vector 
        glRotatef(PI/2,1,1,1);
        
        /*
         fill(fillColor);
         rect(-abs(d.x)/2,-length,abs(d.x),length);
         */
        
		/*
        ofSetColor(c, opacity*255/4);
        ofSetLineWidth(4);
        ofLine(-length*3,0, length/2,0);
        
        ofSetColor(c, opacity*255/1.5);
        ofSetLineWidth(2);
        ofLine(-length/2,0, length/2,0);
		 */
    }
    
//    noStroke();
    
    if(hasTarget)
        ofSetColor(255,0,0,190);
    else
        ofSetColor(fillColor,50);
    
    ofEllipse(0,0,5,5);
    
    ofSetColor(fillColor);
    ofEllipse(0,0,2,2);
    glPopMatrix();
}

