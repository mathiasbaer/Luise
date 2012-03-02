#ifndef schirn_foyer_imagelist_h
#define schirn_foyer_imagelist_h

#include "ofMain.h"

class ImageList {
    
public:
    
    ImageList() {
		
    }
	
	void create();
	ofImage getRandom();
	
	ofDirectory dir;
	std::vector<ofImage> images;
	
};


#endif
