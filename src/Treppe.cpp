#include "Treppe.h"

void Treppe::update() {
    
}

void Treppe::setSeite(int _seite) {
    mAlign = _seite;
}


void Treppe::initStufen(int _w, int _h) {
    for(int i = 0; i < STUFENANZAHL; i++) {
        Stufen[i].init(mPosition, _w, _h);
    }
}

void Treppe::setStufenAbstand(int _abstand) {
    mStufenAbstand = _abstand;
    setPosition(mPosition);
}
void Treppe::setStufenBreite(int _breite) {
    mStufenBreite = _breite;
    for(int i = 0; i < STUFENANZAHL; i++) {
        Stufen[i].mWidth = mStufenBreite;
    }
    setPosition(mPosition);
}

void Treppe::setPosition(ofVec2f _pos) {
    mPosition = _pos;
    
    Stufen[0].mPosition = mPosition;
    for(int i = 1; i < STUFENANZAHL; i++) {
        Stufen[i].mPosition.x = Stufen[i-1].mPosition.x + Stufen[i-1].mWidth;
        
        if(mAlign == 2) { Stufen[i].mPosition.y = Stufen[i-1].mPosition.y - mStufenAbstand; }
        else { Stufen[i].mPosition.y = Stufen[i-1].mPosition.y + mStufenAbstand;  }
        
    }
    
}

void Treppe::draw() {
    

    for(int i = 0; i < STUFENANZAHL; i++) {
        int sX = Stufen[i].mPosition.x;
        int sY = Stufen[i].mPosition.y;
        int sW = Stufen[i].mWidth;
        float sH = Stufen[i].curHeight;
        
        ofSetColor(Stufen[i].mColor.r, Stufen[i].mColor.r, Stufen[i].mColor.r);
        ofRect(sX, sY, sW, sH);
    }

}