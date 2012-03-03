#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    
    //General Settings
    ofSetFrameRate(30);
    //ofSetVerticalSync(true);
    
    //Grafik Settings
    ofEnableAlphaBlending();
    //ofEnableSmoothing();
    ofBackground(0, 0, 0);
    ofSetBackgroundAuto(false);
    
    
    //Videoinput & Tracking
    
    #ifdef _USE_TWO_CAMS
        for (int i = 0; i<2; i++) {
            //752x480
            //camera[i]setSize(700, 200);
            //camera[i].setPosition((752 - 700) / 2, 100);
            
            camera[i].setFrameRate(30);
            camera[i].setImageType(OF_IMAGE_GRAYSCALE);
            camera[i].setFormat7(true);
        }
        
        camera[1].setup("0xB09D0100ADA9AF");
        camera[0].setup("0xB09D0100ADA978");
        
        mCamWidth = camera[0].getWidth();
        mCamHeight = camera[0].getHeight();
        
        mColorImg.allocate(mCamWidth,mCamHeight);
        
        mGrayImage.allocate(mCamWidth*2,mCamHeight);
        mGrayDiff.allocate(mCamWidth*2,mCamHeight);
        mSaveBackground.allocate(mCamWidth*2,mCamHeight);
        
    #else
        
        mCamWidth = 320;
        mCamHeight = 240;
        
        vidGrabber.setVerbose(true);
        vidGrabber.setDeviceID(3);
        vidGrabber.initGrabber(mCamWidth,mCamHeight);
        
        mColorImg.allocate(mCamWidth,mCamHeight);
        mGrayImage.allocate(mCamWidth,mCamHeight);
        mGrayDiff.allocate(mCamWidth,mCamHeight);
        mSaveBackground.allocate(mCamWidth,mCamHeight);
    
    #endif
    
	imageList.create();
	
	attractorGraphics.create();
	
	background.loadImage("images/Bildschirmfoto 2012-03-02 um 19.25.26.png");
	
	screenTop.allocate(2048,768);
	screenLeft.allocate(1048,768);
	screenRight.allocate(1048,768);
	
	//blendmodeShader.load(<#string shaderName#>)
	
	
/*
    //GUI
    //gui.addPage("setup");
        
    gui.addTitle("screen settings");
	gui.addSlider("screen start", scStart, -2000, 2000); 
	gui.addSlider("screen end", scStop, -2000, 2000); 
    gui.addSlider("screen height", scHeight, -200, 1500); 
    
    //VideoSetup
	gui.addTitle("video settings").setNewColumn(true);
	gui.addSlider("threshold", mThreshold, 0, 200);
    gui.addSlider("blur", mBlur, 0, 10);
    gui.addContent("kamera", mColorImg);
	gui.addContent("gray diff", mGrayDiff);
    
    gui.loadFromXML();
	gui.show();
*/

    //Create Fragments
    for (int i=0; i<FRAGMENTNUMBER; i++) {
        fragments[i].create(ofRandomWidth(),ofRandomHeight());
    }
    


    
}

//--------------------------------------------------------------
void testApp::update(){
    
    bool bNewFrame = false;
    
    #ifdef _USE_TWO_CAMS
        for (int i = 0; i<2; i++) {
            if(camera[i].grabVideo(mCaptureImages[i])) {
                camera[i].update();
                bNewFrame = true;
            }
        }
    #else
        vidGrabber.grabFrame();
        //Neuer Frame?
        bNewFrame = vidGrabber.isFrameNew();
    #endif
        
    if (bNewFrame){
                
        #ifdef _USE_TWO_CAMS
                
                
                mCaptureImages[0].setImageType(OF_IMAGE_COLOR);
                ofxCvGrayscaleImage 	tmpGrayImage;
                tmpGrayImage.allocate(mCamWidth, mCamHeight);
                tmpGrayImage.setFromPixels(mCaptureImages[0].getPixels(), mCamWidth, mCamHeight);
                
                mGrayImage.setROI(0, 0, mCamWidth, mCamHeight);
                mGrayImage = tmpGrayImage;
                mGrayImage.resetROI();
                
                mCaptureImages[1].setImageType(OF_IMAGE_COLOR);
                tmpGrayImage.setFromPixels(mCaptureImages[1].getPixels(), mCamWidth, mCamHeight);
                
                mGrayImage.setROI(mCamWidth, 0, mCamWidth*2, mCamHeight);
                mGrayImage = tmpGrayImage;
                mGrayImage.resetROI();
        #else
                
                mColorImg.setFromPixels(vidGrabber.getPixels(), mCamWidth,mCamHeight);
                mGrayImage = mColorImg;
                
        #endif
        
        // take the abs value of the difference between background and incoming and then threshold:
		mGrayDiff.absDiff(mSaveBackground, mGrayImage);
        
        mGrayDiff.blur(mBlur);
		mGrayDiff.threshold(mThreshold);
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		mContourFinder.findContours(mGrayDiff, 20, (mCamWidth*mCamHeight)/3, 10, false);	// find holes
        
        
        
        /////////////////////////////////////////////////
        // Tracking /////////////////////////////////////
        /////////////////////////////////////////////////
        
        if(mTracking){
            for (int i = 0; i < mContourFinder.nBlobs; i++) {
                ofxCvBlob tmpBlob = mContourFinder.blobs[i]; 
                                
                bool newBlob = true;
                for (int a=0; a< mLastBlobs.size(); ++a) {
                    float distance = mLastBlobs[a].centroid.distance(tmpBlob.centroid);
                    if(distance <= 20) {
                        newBlob = false;
                        break;
                    }	
                }
                
                if(newBlob) {
                    //Trackingpoint erstellen
                    trackingPoints.push_back(TrackingPoint(tmpBlob.centroid, mCamWidth, mCamHeight));
                }
  
            } //i-schleife
            
            //Neue Blobliste erstellen
            mLastBlobs.clear();
            
            for (int i = 0; i< mContourFinder.nBlobs; i++) {
                mLastBlobs.push_back(mContourFinder.blobs[i]);  
            }
                      
            
            //TrackingPoints
            vector<int>blobIDs;
            
            if(mLastBlobs.size() == 0) {
                trackingPoints.clear();
            } else {
                
                //cout << "TP: " << trackingPoints.size() << " // Blobs: " << mLastBlobs.size() << endl;
                for (int i = 0; i<trackingPoints.size(); i++) {
                    
                    float nearest = 0;
                    int nearestID = 0;
                    
                    for (int n = 0; n < mLastBlobs.size(); n++) {
                        //Check Abstand zwischen Blob und Trackpoint
                        float distance = trackingPoints[i].checkDist(mLastBlobs[n].centroid);
                        
                        if( n == 0 ) {
                            nearest = distance;
                            nearestID = n;
                        } else {
                            if(distance <= nearest) {
                                nearest = distance;
                                nearestID = n;
                            }
                        }
 
                    }    
                    
                    bool foundBlobID = false;
                    for (int n = 0; n < blobIDs.size(); n++) {
                        if(blobIDs[n] == nearestID) {
                            foundBlobID = true;
                            n = blobIDs.size();
                        } 
                    }
                    
                    if(foundBlobID) {
                        //Blob wird schon von TrackPoint verfolgt..
                        //Delete trackingPoint
                        std::vector<TrackingPoint>::iterator iter = trackingPoints.begin() + i;
                        i--;
                        trackingPoints.erase(iter);
                    } else {
                        blobIDs.push_back(nearestID);
                        trackingPoints[i].update(mLastBlobs[nearestID].centroid);
                    }
                     
                }
            } //IF blobs = 0
        } //mTracking
    } //bNewFrame


    //Background Speichern
    if(mSavePicture) { 
        mSaveBackground = mGrayImage; 
        mSavePicture = false;
    }
    
    /////////////////////////////////////////////////
    // Sound ////////////////////////////////////////
    /////////////////////////////////////////////////	
    ofSoundUpdate();
    
    mSoundControl.update();
    
    
    
    /////////////////////////////////////////////////
    // Structurs ////////////////////////////////////
    /////////////////////////////////////////////////
    
    int numberOfTP = trackingPoints.size();
    //Reset Verbindungen zwischen Trackingpoints & Structure
    for (int i = 0; i<numberOfTP; i++) {    
        trackingPoints[i].hasStructure = false;
    }
	
	for (int i=0; i<structures.size(); i++) {
        
        //Wenn die Structur auf DoDelete steht -> lšschen
        if (structures[i].mDoDelete) {
            structures[i].destroy();
            structures.erase(structures.begin() + i);
            i--;
            continue;
        }
        
        //Keine TrackingPunkte vorhanden
        if(numberOfTP == 0) {
            structures[i].setTrackingPoint(false);
            structures[i].update();
            continue;
        } 
        
        //Trackingpoints Suchen
        float nearest = 0;
        int nearestID = -1;
        for (int n = 0; n < trackingPoints.size(); n++) {
            //Abstand Trackingpunkt <-> Struktur
            
            if(trackingPoints[n].hasStructure) { continue; }
            
            float distance = trackingPoints[n].checkDist(structures[i].mTrackingPointPosition);
            if( n == 0 ) {
                nearest = distance;
                nearestID = n;
            } else {
                if( distance <= nearest ) {
                    nearest = distance;
                    nearestID = n;               
                }
            }   
        }
        
        if (nearestID == -1) {
            //keine freien TrackingPoints mehr...
            structures[i].setTrackingPoint(false);
        }
        else {
            //TrackingPosition Ÿbergeben
            trackingPoints[nearestID].hasStructure = true;
            structures[i].setTrackingPoint(true, trackingPoints[nearestID].mMapPos);
            
        }
        
        //Structuren Update
        structures[i].update();
        
    }
    
    //Structuren erstellen wenn TP ohne Structur
    for (int n = 0; n < trackingPoints.size(); n++) {
        if (!trackingPoints[n].hasStructure && structures.size() < trackingPoints.size() ) {
            createStructure(trackingPoints[n].mMapPos, ofRandom(50,70));
        }
    }
	
		
    
    //Update Fragments
    for ( int i=0; i<FRAGMENTNUMBER; i++ ) {
		int attIdx = fragments[i].findAttractor( attractors );
		if ( attIdx > -1 ) {
			attractors[attIdx].addSatellite(fragments[i].position);
		}
		fragments[i].update();
    }
	
	// upadate attractors
	for ( int i=0; i<attractors.size(); i++ ) {
        attractors[i].update();
		
		Attractor a = attractors[i];
		if (a.hasEnough()) {
			changeGraphic( a.satellites );
		}
		
		attractors[i].clearSatellites();
    }
	
	
	
	
	/////////////////////////////////////////////////
    // GraphicsContainer ////////////////////////////
    /////////////////////////////////////////////////	

    attractorGraphics.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	
	//screenTop.begin();
	
	ofEnableAlphaBlending();
    ofSetColor(255,255,255,20);
    //ofRect(0, 0, ofGetWidth(), ofGetHeight());
	background.draw(0, 0, 1024, 768);

	ofDisableAlphaBlending();
	
    float ch = mCamWidth;
    float cw = mCamWidth;
    int breite = scStop-scStart;
    int hoehe = (int) ((ch/cw)*breite);
    ofRectangle screenRect;
    screenRect.set(scStart, scHeight-hoehe, breite, hoehe);
   

	

    if(setupMode) {

        //Draw Camera & Blobs
        ofSetColor(255,255,255);
        
        mGrayImage.draw(screenRect);
        mContourFinder.draw(screenRect);
        
        // finally, a report:

        ofSetColor(0,255,0);
        ofDrawBitmapString("press s: kamerasetup\npress f: toggle fullscreen", 20, 650);
    
        ofSetColor(255,255,255);
        char reportStr[1024];
        sprintf(reportStr, "bg subtraction and blob detection\nthreshold %i \nnum blobs found %i, fps: %f", mThreshold, mContourFinder.nBlobs, ofGetFrameRate());
        ofDrawBitmapString(reportStr, 20, 600);
    }
    
    //Streifen
    
    /*
    ofSetColor(255, 0, 0, 50);
    for (int i = 0; i < contourFinder.nBlobs; i++){
        ofxCvBlob tmpBlob = contourFinder.blobs[i];
        int blobXMapped = ofMap(tmpBlob.centroid.x, 0, CAMWIDTH, scStart, scStop);
        int blobTopMapped = ofMap(tmpBlob.boundingRect.y, 0, CAMHIGHT, 0, screenRect.height);
        
        ofFill();
        ofRect(blobXMapped, 0, 20, scHeight-blobTopMapped);
    }*/
    
    
   
    int ln = structures.size();
	for (int i=0; i<ln; i++) {
        structures[i].draw();
	}

    
    //GUI
    //gui.draw();
    
    
    if(!setupMode) {
        //Draw Fragments
		
		// only for testing. not visible
		for ( int i=0; i<attractors.size(); i++ ) {
			//attractors[i].draw();
		}
		
        for ( int i=0; i<FRAGMENTNUMBER; i++ ) {
            fragments[i].draw();
		}
   
        
        
        ofSetColor(255);
        #ifdef _USE_TWO_CAMS
            mGrayDiff.draw(0, 0, mCamWidth, mCamHeight/2);
            mContourFinder.draw(0,0,mCamWidth, mCamHeight/2);
            mGrayImage.draw(mCamWidth+20,0,mCamWidth, mCamHeight/2);
        #else
            mGrayDiff.draw(0, 0, mCamWidth, mCamHeight);
            mContourFinder.draw(0,0,mCamWidth, mCamHeight);
            mGrayImage.draw(mCamWidth+20,0,mCamWidth, mCamHeight);
        #endif	
        
		

		
    }
    

     
    //Trackingpoints zeichnen
    int ln_tp = trackingPoints.size();
    for (int i = 0; i<ln_tp; i++) {
        trackingPoints[i].draw();
        
    }

	// GrafphicsContainer
	attractorGraphics.draw();
	
	/*
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	ofEnableAlphaBlending();
	ofSetColor(255, 0, 0, 20);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	ofDisableAlphaBlending();
	ofDisableBlendMode();
	*/
	
	/*
	screenTop.end();
	
	
	ofSetColor(255, 255, 255);
	screenTop.draw(0,0);
	 */
}





void testApp::createStructure(ofVec2f _pos, int _n) {

	//////////////////
    // Sind die Fragments dann nicht irgendwann von den Positionen so geordent, dass sie immer an richtiger stelle stehen?
    
	//cout << "---> create structure" << endl;
	
	std::vector<Fragment*> tmp;
	
	int s = ofRandom( FRAGMENTNUMBER - _n );
	
	for (int i=s; i<FRAGMENTNUMBER && tmp.size() < _n; i++) {
		//Fragment f = fragments[i];
		if (!fragments[i].hasTarget) {
			tmp.push_back(&fragments[i]);
			//cout << "frag id" << &fragments[i] << endl;
		}
	}

	
	//Erst Objekt in Vector erstellen. Objekt wird intern im Vector umkopiert
	structures.push_back(Structure());
    //Deswegen erst dannach auf den Vector zugreifen!
    structures.back().create(_pos.x,_pos.y,tmp);

}


void testApp::createAttractor() {
	attractors.push_back(Attractor());
	attractors.back().create();
}

void testApp::changeGraphic( std::vector<ofVec2f> _v ) {
	
	attractorGraphics.init(imageList.getRandom(),0,0);
	attractorGraphics.setVertices(_v);
}

static float adder = 0;
//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
	// samples are "interleaved"
	for (int i = 0; i < bufferSize; i++){
		left[i] = input[i*2];
		//right[i] = input[i*2+1];
		right[i] = left[i];
	}
	bufferCounter++;
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case 'a':
            createAttractor();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'e':
            //gui.toggleDraw();
            setupMode =! setupMode;
            
            break;
        case 'u':
            scStart = 0;
            //gui.saveToXML();
            break;
		case 'p':
            //startTracking = true;
			createStructure(ofVec2f(mouseX, mouseY), 70);
			break;
        case ' ':
            mSavePicture = true;
            break;
        case 't':
            mTracking = !mTracking;
            break;
        case 'i':
            mThreshold--;
            if(mThreshold < 0) mThreshold = 0;
            cout << "Threshold: " << mThreshold << endl;
            break;
        case 'o':
            mThreshold++;
            cout << "Threshold: " << mThreshold << endl;
            break;
        case 'k':
            mBlur--;
            if(mBlur < 0) mBlur = 0;
            cout << "Blur: " << mBlur << endl;
            break;
        case 'l':
            mBlur++;
            cout << "Blur: " << mBlur << endl;
            break;

	}
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

    //objekt in liste erstellen
    //vieleobjekte.push_back(DisplayObject());
    //objekt loschen
    //vieleobjekte.pop_back();


}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	std::vector<ofVec2f> tmp;
	for (int i=0; i<20; i++) {
		tmp.push_back(ofVec2f(ofRandom(ofGetWidth()),ofRandom(ofGetHeight())));
	}
	changeGraphic( tmp );
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
