#include "Structure.h"


void Structure::create(int _id) {
	structID = _id;
}

void Structure::build(std::vector<Fragment> _fs) {
	children = _fs;
	
	leader.create();

	for ( int i=0; i<children.size(); i++ ) {
		Fragment child = children[i];
		
		if (i > 0)
			// pass fragment id because passing a fragment object does not work
			child.setTarget( children[i-1].fragID );
		else
			// same as fragment. pass structure id and true for follow leader
			child.setTarget( structID, true );
		
		//child.setStructure(this);
		child.setPropertiesWithIndex( i );
    }
}

void Structure::destroy() {
}
