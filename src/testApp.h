#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"
#include "ofxOpenCv.h"

#include "displayObject.h"
#include "fragment.h"
#include "Structure.h"
#include "TrackingPoint.h"
#include "ImageList.h"
#include "GraphicsContainer.h"
#include "Attractor.h"
#include "MeshObject.h"

//CONST
#define RECORDPICTURES 8
#define FRAGMENTNUMBER 500
#define STRUCTURENUMBER 20
#define CAMWIDTH 320  //MUSS AUCH IN TRACKINGPOINT.H GE€NDERT WERDEN
#define CAMHEIGHT 240 //MUSS AUCH IN TRACKINGPOINT.H GE€NDERT WERDEN



class testApp : public ofBaseApp{
    
	public:
    
        testApp() {
            
            //Videoinput & Tracking
            mSavePicture = false;
            mTracking = false;
            
            //Set Screen
            setupMode = true;
			
			drawBuffer = false;
			
			structures.reserve(STRUCTURENUMBER);
			
			mThreshold = 9;
			mBlur = 8;
            
        }
    
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
	
		void createStructure(ofVec2f _pos, int _n);
		void createAttractor();
    

        //Camera & Tracking
        ofVideoGrabber          vidGrabber;
	
		ofImage background;
        
        ofxCvColorImage			mColorImg;
        ofxCvGrayscaleImage 	mGrayImage;
		ofxCvGrayscaleImage 	mGrayDiff;
        ofxCvGrayscaleImage 	mSaveBackground;

        ofxCvContourFinder      mContourFinder;
    
    
        int         mThreshold;
        int         mBlur;
        bool        mSavePicture;
        bool        mTracking;
    
        vector <ofxCvBlob> mLastBlobs;
        std::vector<TrackingPoint> trackingPoints;
		
		// all images in bin/data/images. images folder not syncing with git somehow
		ImageList imageList;
	
		// graphics: make vector/array later
		GraphicsContainer graphic;
		bool drawBuffer;
    
        //Fragmente & Structuren
        Fragment fragments[FRAGMENTNUMBER];
		std::vector<Structure> structures;
	
		std::vector<Attractor> attractors;
		
		ofShader blendmodeShader;

        //Setup Screen
        bool        setupMode;
        int         scStart;
        int         scStop;
        int         scHeight;

    
    
};

