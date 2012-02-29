#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"

#include "displayObject.h"
#include "fragment.h"
#include "Structure.h"
//#include "FocusObject.h"
#include "TrackingPoint.h"

//CONST
#define RECORDPICTURES 8
#define FRAGMENTNUMBER 30
#define CAMWIDTH 320
#define CAMHIGHT 240


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
		void createStructure(float _x, float _y, int _n);
    
        //2 Bilder Addieren
        unsigned char*  maxImage( ofxCvGrayscaleImage& mom, ofxCvGrayscaleImage& dad );

        ofVideoGrabber 		vidGrabber;


        ofxCvColorImage			colorImg;

        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayDiff;

        ofxCvContourFinder 	contourFinder;
    
        //Tracking
        ofxCvGrayscaleImage     arrSavePictures[RECORDPICTURES];
        ofxCvGrayscaleImage 	allDiff;

        bool speichern;
		
        //Fragmente
        Fragment tmpFragment;
        Fragment fragments[FRAGMENTNUMBER];
	
		Structure tmpStructure;
		std::vector<Structure> structures;
	
		TrackingPoint tmpTrackingPoint;
		std::vector<TrackingPoint> trackingPoints;
        
        //Setup Screen
        bool        setupMode;
        int         scStart;
        int         scStop;
        int         scHeight;

        int         threshold;
        int         blur;
    
};

