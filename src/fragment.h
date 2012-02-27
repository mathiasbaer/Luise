#ifndef schirn_foyer_fragment_h
#define schirn_foyer_fragment_h


#include "ofMain.h"
#include "displayObject.h"
#include "LFO.h"

class Fragment: public DisplayObject {
    
public:
    
    // Interpolations
    // x and y in relation to rotation if in structure mode
    
    Fragment() {
        //LFO iRotation = new LFO;
        //iLength     = new LFO();
        
        opacity     = 0;
        
        //lfos[]      = { iRotation, iLength };
    }
    
     
    //LFO iRotation;
    LFO iLength;
    
    LFO lfos[2];
    
    
    // appearance
    float opacity;
    
};



#endif
