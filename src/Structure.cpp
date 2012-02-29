#include "Structure.h"

void Structure::build( float _posX, float _posY, std::vector<Fragment*> _fs ) {
    
    
	children = _fs;
        
    //zufall
    //leader.attach();
    //positionierung
    leader.attach(_posX, _posY);
	    
	int vecLength = children.size();
	
    for ( int i=0; i<vecLength; i++ ) {
		
		Fragment* child = children[i];
		
		if (i > 0)
			// pass fragment id because passing a fragment object does not work
            child->setTarget( children[i-1] );
		else
			// same as fragment. pass structure id and true for follow leader
			child->setTarget( &leader, true );
		
		child->setPropertiesWithIndex( i );
    }
    
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
