#ifndef schirn_foyer_fragment_h
#define schirn_foyer_fragment_h

#include "ofMain.h"
#include "displayObject.h"
#include "FocusObject.h"
#include "LFO.h"
#include "Attractor.h"

class Fragment: public DisplayObject {
    
public:
    
    Fragment(): DisplayObject() {
        
        opacityActive     = 0;
		opacityBase		  = 0;
		opacity			  = opacityBase;
        
    }
    
    // Interpolations
    // x and y in relation to rotation if in structure mode
    
    
    void create(float _x, float _y);
    void update();
	int  findAttractor( std::vector<Attractor> _atts );
    void setPropertiesWithIndex( int _i );
    void draw();
    
    LFO iRotation;
    LFO iLength;
	LFO iOpacity;
    
    // appearance
    float opacityActive;
	float opacity;
	float opacityBase;
    
    
    
};



#endif
