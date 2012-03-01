#include "Structure.h"

void Structure::create( float _posX, float _posY, std::vector<Fragment*> _fs ) {
    
	children = _fs;
	leader.attach(_posX, _posY);
    
	setBindings();
    
}

void Structure::setBindings() {
	
	int vecLength = children.size();
	for ( int i=0; i<vecLength; i++ ) {
		
		Fragment* child = children[i];
		
		if (i > 0) {
			// pass fragment id because passing a fragment object does not work
            child->setTarget( children[i-1] );
		} else {
			// same as fragment. pass structure id and true for follow leader
			child->setTarget( &leader );
		}
		child->setPropertiesWithIndex( i );
    }
}

void Structure::addChildren( std::vector<Fragment*> _fs ) {
	
	int ln = _fs.size();
	for (int i=0; i<ln; i++) {
		children.push_back(_fs[i]);
	}
	setBindings();
}

void Structure::draw() {
	
    leader.draw();
	
	int ln = children.size();
	
	for (int i=0; i<ln-1; i++) {
		Fragment* f0 = children[i];
		
		for (int j=i+1; j<ln; j++) {
			Fragment* f1 = children[j];
			
			float dist = f0->position.distance(f1->position);
			
			if (dist<100 && f0->opacity < 0.8) {
				float op = (1-dist/100);
				if (op>f0->opacity) op = f0->opacity;
				ofSetColor(f0->fillColor, op*250);
				ofLine(f0->position.x,f0->position.y,f1->position.x,f1->position.y); 
			}
		}
	}
}

//void Structure::update(float _x, float _y) {
//	leader.update(_x, _y);
//}

void Structure::update(std::vector<TrackingPoint> *_tpList) {

}

void Structure::destroy() {
    
    //for ( int i=0; i<children.length; i++ ) {
    //    children[i].clearTarget();
    //} 
}
