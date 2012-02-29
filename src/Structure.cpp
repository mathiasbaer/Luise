#include "Structure.h"

void Structure::build( float _posX, float _posY, int _size, std::vector<Fragment> *_fs ) {
    
    
    std::vector<Fragment> *children = _fs;
        
    leader.create();
    
    for ( int i=0; i<_size; i++ ) {
		Fragment child = children->at(i);
		
		if (i > 0)
			// pass fragment id because passing a fragment object does not work
            child.setTarget( &children->at(i-1) );
		else
			// same as fragment. pass structure id and true for follow leader
			child.setTarget( &leader, true );
		
		child.setPropertiesWithIndex( i );
    }
    
}

void Structure::destroy() {
    
    //leader = null;
    //for ( int i=0; i<children.length; i++ ) {
    //    children[i].clearTarget();
    //} 
    
    
}
