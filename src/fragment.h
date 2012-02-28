#ifndef schirn_foyer_fragment_h
#define schirn_foyer_fragment_h

#include "ofMain.h"
#include "displayObject.h"
#include "LFO.h"

class Fragment: public DisplayObject {
    
public:
    
    Fragment(): DisplayObject() {
        
        lfos[0] = iRotation;
        lfos[1] = iLength;
        
        opacity     = 0;
        
    }
    
    // Interpolations
    // x and y in relation to rotation if in structure mode
    
    
    void create(float _x, float _y, int _id);
    void update(std::vector<Fragment> _fragments);
    void setPropertiesWithIndex( int _i );
    void draw();
    LFO iRotation;
    LFO iLength;
    LFO lfos[2];
    // appearance
    float opacity;
	int fragID;
    
    
    
};



#endif
