#ifndef schirn_foyer_attractor_h
#define schirn_foyer_attractor_h

#include "ofMain.h"
#include "displayObject.h"
#include "LFO.h"

class Attractor: public DisplayObject {
    
public:
    
    Attractor(): DisplayObject() {
        
    }
    
    void create();
    void update();
    void draw();
};



#endif