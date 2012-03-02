#ifndef schirn_foyer_meshobject_h
#define schirn_foyer_meshobject_h

#include "ofMain.h"
#include "fragment.h"


class MeshObject {
    
public:
    
	MeshObject() {    

    }
	
	void create();
	void update();
	void draw();
	void setFragments(std::vector<Fragment*> _fs);
	
	std::vector<Fragment*> nodes;
};



#endif

