#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#include "displayObject.h"
#include "fragment.h"
#include "Structure.h"
#include "TrackingPoint.h"
#include "ImageList.h"
#include "GraphicsContainer.h"
#include "Attractor.h"
#include "MeshObject.h"
#include "ofxLibdc.h"
#include "Treppe.h"

//CONST
#define RECORDPICTURES 8
#define FRAGMENTNUMBER 500
#define STRUCTURENUMBER 20
#define LINKS 1
#define RECHTS 2

// Wenn 2 Kameras
//#define _USE_TWO_CAMS

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
            
            mVolume = 0;
            mSmoothVolume = 0;
            
            hspeed=0;
            lastvolume=0;
            baaam=0;
            
            
            mMinBlobsize = 500;
            mMaxBlobs = 10;
            
            modus = 1;
            
            mTrackingFrame = 0;
            
            keyOption = 0;
            mouseClickStart = ofVec2f(0,0);
        }
    
		void setup();
		void update();
		void draw();
    
        void loadSettings();
        void saveSettings();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void changeGraphic( std::vector<ofVec2f> _v );
	
		void createStructure(ofVec2f _pos, int _n);
		void createAttractor();
    
        //SOUND
        void audioReceived 	(float * input, int bufferSize, int nChannels); 

        float * left;
        float * right;
        int 	bufferCounter;

        int nBandsToGet;
        float * fftSmoothed;

    
        float maxLevel;
        float peaks[100];
	

        float mVolume;
        float mSmoothVolume;
    
        //speeder helper
        int hspeed;
    
        float lastvolume;
        float baaam;
    
        //Camera & Tracking
            
        int mCamWidth;
        int mCamHeight;
            
        #ifdef _USE_TWO_CAMS
            ofxLibdc::PointGrey camera[2];
            ofImage mCaptureImages[2];
            // include these just to make sure they compile
            ofxLibdc::Grabber   dummyGrabber;
        #else
            ofVideoGrabber      vidGrabber;
        #endif
	
		
        
        ofxCvColorImage			mColorImg;
        ofxCvGrayscaleImage 	mGrayImage;
		ofxCvGrayscaleImage 	mGrayDiff;
        ofxCvGrayscaleImage 	mSaveBackground;

        ofxCvContourFinder      mContourFinder;
    
    
        int         mThreshold;
        int         mBlur;
        bool        mSavePicture;
        bool        mTracking;
        int         mMinBlobsize;
        int         mMaxBlobs;
        int         mTrackingFrame;
    
        //TEMP
        ofImage background;
        
        vector <ofxCvBlob> mLastBlobs;
        std::vector<TrackingPoint> trackingPoints;
		
		ofFbo screenTop;
		ofFbo screenLeft;
		ofFbo screenRight;
	
		// all images in bin/data/images. images folder not syncing with git somehow
		ImageList imageList;
	
		// attractorGraphicss: make vector/array later
		GraphicsContainer attractorGraphics;
		std::vector<GraphicsContainer> attractorGraphicss;
		bool drawBuffer;
    
        //Fragmente & Structuren
        Fragment fragments[FRAGMENTNUMBER];
		std::vector<Structure> structures;
	
		std::vector<Attractor> attractors;
		
		//ofShader blendmodeShader;

        //Setup Screen
        bool        setupMode;
        int         modus;
        int         lastKey;
        int         keyOption;
        int         scStart;
        int         scStop;
        int         scHeight;
        ofVec2f     mouseClickStart;

    
        //Untere Screens
        Treppe      mTreppen[2];
    
};

