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

void Structure::update(float _x, float _y) {
	leader.update(_x, _y);
}

void Structure::destroy() {
    
    //leader = null;
    //for ( int i=0; i<children.length; i++ ) {
    //    children[i].clearTarget();
    //} 
    
    
}
