#ifndef LFO_1
#define LFO_1

#include "ofMain.h"

class LFO {
    
public:
   
    float radians;
    float speed;
    
    // result
    float value;
    float baseValue;
    float minValue;
    float maxValue;
    
    //----------------------------------------
   
    LFO() {
        radians = 0;
        speed = 0;
        
        // result
        value = 0;
        
        baseValue = 0;
        range = 0;
        minValue = 0;
        maxValue = 0; 
    }
    
    //----------------------------------------
    
    void setRange( float r ) {
        range    =  r;
        minValue = -r/2;
        maxValue =  r/2;
    }
    
    void update () {
        radians += speed;
        //value = ( sin(radians) * maxValue ) * 2 + minValue + baseValue;
        value = ( (sin(radians) + 1) / 2 ) * range + minValue + baseValue;
    }

private:
    float range;
    
};


#endif
