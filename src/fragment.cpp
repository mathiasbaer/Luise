#include "fragment.h"


void Fragment::create(float _x, float _y, int _id) {
   
    fragID = _id;
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

void Fragment::update(std::vector<Fragment> _fragments) {
    
    if( hasTarget || opacity > 0 ) {
        for ( int i=0; i<sizeof(lfos); i++ ) { lfos[i].update(); }
    }
 
    
    // is in structure
    if( hasTarget ) {
		Fragment target = _fragments[targetID];
		
		/*
        // current fragment
        ofVec2f v0 = position;
        // following this fragment
        ofVec2f v1 = target.desired;
        
        // get desired position in structure => v1
        rotation = iRotation.value + target.rotation;
        ofVec2f offset;
		offset.set( cos(rotation), sin(rotation) );
        offset.mult( offsetToTarget );
        v1.add( offset );
        
        
        desired = ofVec2f.sub(v1, v0);
        
        float d = desired.mag();
        targetDistance = d;
        
        if (d>0) {
            desired.normalize();
            if (d < 50.0f) desired.mult(2*(d/50.0f));
            else desired.mult( 2 );
            
            force = PVector.sub(desired,vel);
            force.mult( 0.05 );
            
            acc.add( force );
            acc.limit( 2 );
        }
        else {
            acc = new PVector( 0, 0 );
        }
        
        vel.add(acc);
        position.add(vel);
        
        acc.mult( 0.2 );
        
        // set position in structure as desired for next fragment
        desired = v1.get();
        
        // set force to move fragment up if target is cleared next frame
        force = new PVector(0,-1.5);
        force.add(vel);
        force.limit( 2 );
		 */
     
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
        
//        stroke(c, opacity*255/4);
//        strokeWeight(4);
//        line(-length*3,0, length/2,0);
        
//        stroke(c, opacity*255/1.5);
//        strokeWeight(2);
//        line(-length/2,0, length/2,0);
    }
    
//    noStroke();
    ofSetColor(fillColor,50);
    ofEllipse(0,0,5,5);
    
    ofSetColor(fillColor);
    ofEllipse(0,0,2,2);
    glPopMatrix();
}

