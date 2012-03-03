#ifndef schirn_foyer_Treppe_h
#define schirn_foyer_Treppe_h

#include "ofMain.h"
#include "Stufe.h"

//CONST
#define STUFENANZAHL 20

class Treppe {
    
public:
    
    //mAlign 1 = links
    //mAlign 2 = rechts
    
    Treppe() {
        
       
        mStufenAbstand = 20;
        mStufenBreite = 40;
        mStufenHoehe = 60;
        mPosition = ofVec2f(0,0);
        
        initStufen(mStufenBreite, mStufenHoehe);
        //setPosition(mPosition);
    }
    
    void update();
    void setSeite(int _seite);
    void initStufen(int _w, int _h);
    void setPosition(ofVec2f _pos);
    void setStufenAbstand(int _abstand);
    void setStufenBreite(int _breite);
    void draw();
    
    
    
    ofVec2f mPosition;
    Stufe Stufen[STUFENANZAHL];
    ofColor mColor[STUFENANZAHL];
    int mStufenAbstand;
    int mStufenBreite;
    int mStufenHoehe;
    int mAlign;
	
};


#endif

