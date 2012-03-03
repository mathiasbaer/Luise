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
    
    
    //Sound Input
	ofSoundStreamSetup(0,2,this, 44100, 256, 4);	
	left = new float[256];
	right = new float[256];
	bufferCounter = 0;
    
	maxLevel = 30;
	for (int i = 0; i<100; i++) {
		peaks[i] = 30;
	}

    //smooth volume
    nBandsToGet = 128;
	fftSmoothed = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}

    
    
	imageList.create();
	
	attractorGraphics.create();
	
	background.loadImage("images/Bildschirmfoto 2012-03-02 um 19.25.26.png");
	
	screenTop.allocate(2048,768);
	screenLeft.allocate(1048,768);
	screenRight.allocate(1048,768);
	
	//blendmodeShader.load(<#string shaderName#>)
	

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
		mContourFinder.findContours(mGrayDiff, mMinBlobsize, (mCamWidth*mCamHeight)/3, mMaxBlobs, false);	// find holes
        
        
        
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
                    int lastTracking = ofGetFrameNum() - mTrackingFrame;
                    if(lastTracking > 10) {
                        //Trackingpoint erstellen
                        trackingPoints.push_back(TrackingPoint(tmpBlob.centroid, mCamWidth, mCamHeight));
                        mTrackingFrame = ofGetFrameNum();
                    }
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
        ofImage saveImg;
        saveImg.setFromPixels(mSaveBackground.getPixels(), mCamWidth, mCamHeight, OF_IMAGE_GRAYSCALE);
        saveImg.saveImage("trackingBackground.png");
        mSavePicture = false;
    }
    
    /////////////////////////////////////////////////
    // Sound ////////////////////////////////////////
    /////////////////////////////////////////////////	

    
    //Volume Input    
    int counter = 0;
    mVolume = 0;
    for (int i = 0; i < 256; i++){
        if(left[i] > 0)
        {
            mVolume += left[i];
            counter++;
        }
        if(right[i] > 0)
        {
            mVolume += right[i];
            counter++;
        }
		
    }
    mVolume = mVolume/counter * 1000;
     
    //Smooth Volume Input
    float * val = right;
    for (int i = 0;i < nBandsToGet; i++){
        
        // let the smoothed calue sink to zero:
        fftSmoothed[i] *= 0.96f;
        
        // take the max, either the smoothed or the incoming:
        if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
        
    }
    
    float dvol = 0;
    for (int i = 0; i<3; i++) {
        dvol += fftSmoothed[i];
    }
    
    mSmoothVolume = dvol*100; 
    //cout << mSmoothVolume << " - " << mVolume << endl;
    
    
	
    // Level spitzen errechnen
    if(ofGetFrameNum() > 10) {
        if (maxLevel < mVolume || (ofGetFrameNum() % 300) == 0) {
            for (int i = 0; i<99; i++) {
                peaks[i] = peaks[i+1];
            }
            peaks[99] = mVolume;
			
            // Durchschnitt berechnen
			
            float newLevel = 0;
            for (int i = 0; i < 100; i++) {
                newLevel += peaks[i];
            }
            maxLevel = newLevel / 100;
        }
    }
	
    //LEVEL MAPPEN /////////////////////////////////////
    float mapLevel = ofMap(mVolume, 0, maxLevel, 0, 8, true);
    
    if(ofGetFrameNum()%10==0) lastvolume=ofMap(mVolume, 0, maxLevel, 0, 10, true);
    
    //speeder einschalten ///////////////////////////////////
    
    if(hspeed==0) {
      if( (ofRandom(0,10)<3)  &&  (mapLevel > 9.5)) hspeed=30;  
    }
    if(hspeed<-100) hspeed=0;
    if(hspeed>-100) --hspeed;
    
    
    
    
    
    
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
    
    if(!setupMode) {
        
       int ln = structures.size();
       for (int i=0; i<ln; i++) {
           structures[i].draw();
       }
       
       
       
       // only for testing. not visible
       for ( int i=0; i<attractors.size(); i++ ) {
           //attractors[i].draw();
       }
       
       for ( int i=0; i<FRAGMENTNUMBER; i++ ) {
           fragments[i].draw();
       }      
		
       // GrafphicsContainer
       attractorGraphics.draw();
		
    }
	
	/*
	ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
	ofEnableAlphaBlending();
	ofSetColor(255, 0, 0, 20);
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	ofDisableAlphaBlending();
	ofDisableBlendMode();

	screenTop.end();
	
	ofSetColor(255, 255, 255);
	screenTop.draw(0,0);
	 */
    
    
    // SETUP /////////////////////////////
    
    if(setupMode) {
        
        //Draw Camera & Blobs
        ofSetColor(255,255,255);
        
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
        
        
        //Trackingpoints zeichnen
        int ln_tp = trackingPoints.size();
        for (int i = 0; i<ln_tp; i++) {
            trackingPoints[i].draw();
            
        }
        
        
        // finally, a report:
        
        ofSetColor(0,0,0);
        ofRect(0, 530, 600, 180);
        ofSetColor(0,255,0);
        ofDrawBitmapString("press ' ': load Settings\n"
                           "press s:   save Settings\n"
                           "press b:   save Camera Picture\n"
                           "press f:   toggle Fullscreen\n"
                           "press m:   modus ( 1 / 2 / 3 )\n\n"
                           "press e:   toggle SetupMode\n"
                           "press z:   threshold -/+\n"
                           "press u:   blur -/+\n"
                           "press i:   minBlobSize -/+\n"
                           "press o:   maxBlobs -/+\n"
                           "press t:   start Tracking\n" , 20, 550);
        
        
        
        ofSetColor(255,255,255);
        char reportStr[1024];
        sprintf(reportStr, "STATUS\n\n"
                            "threshold %i \n"
                            "blur %i \n"
                            "minBlobSize %i\n"
                            "maxBlobs %i\n"
                            "blobs found %i\n"
                            "fps: %f", mThreshold, mBlur, mMinBlobsize, mMaxBlobs, mContourFinder.nBlobs, ofGetFrameRate());
        ofDrawBitmapString(reportStr, 300, 550);
    }
    
    
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

void testApp::loadSettings() {
    ofstream schreiben;
	schreiben.open("settings.txt");
	schreiben << mThreshold << ";";
	schreiben << mBlur << ";";
    schreiben << mMinBlobsize << ";";
    schreiben << mMaxBlobs << ";";
	schreiben.close();
    
    ofImage loadImg;
    loadImg.setImageType(OF_IMAGE_GRAYSCALE);
    loadImg.loadImage("trackingBackground.png");

    mSaveBackground.setFromPixels(loadImg.getPixels(), loadImg.width, loadImg.height);
    
}

void testApp::saveSettings() {
    ifstream lesen;
	string data;
	lesen.open("settings.txt");
	getline(lesen,data,';');
	mThreshold = ofToFloat(data);
	getline(lesen,data,';');
	mBlur = ofToFloat(data);
    getline(lesen,data,';');
	mMinBlobsize = ofToFloat(data);
    getline(lesen,data,';');
	mMaxBlobs = ofToFloat(data);
	lesen.close();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case 'a':
            createAttractor();
            break;
		case 'p':
            //startTracking = true;
			createStructure(ofVec2f(mouseX, mouseY), 70);
			break;
        ///////// AB HIER SINNVOLL !!!!! ////////
        case 'f':      
            ofToggleFullscreen();
            break;
        case 'e':
            setupMode =! setupMode;
            break;
        case ' ':
            loadSettings();
            break;
        case 's':
            saveSettings();
            break;    
        case 'b':
            mSavePicture = true;
            break;    
        case 't':
            mTracking = !mTracking;
            break;
        case 'z': // Threshold
        case 'u': // Blur
        case 'i': // MinBlobSize
        case 'o': // MaxBlobs
        case 'm': // MODUS
            lastKey = key;
            break;
        case '+':
            if(lastKey == 'z') {  mThreshold++; }
            else if(lastKey == 'u') {  mBlur++; }
            else if(lastKey == 'o') {  mMaxBlobs++; }
            else if(lastKey == 'i') {  mMinBlobsize+=5; }
        break;
        case '-':
            if(lastKey == 'z') {  mThreshold--; if(mThreshold < 0) mThreshold = 0; }
            else if(lastKey == 'u') {  mBlur--; if(mBlur < 0) mBlur = 0; }
            else if(lastKey == 'o') {  mMaxBlobs--; if(mMaxBlobs < 0) mMaxBlobs = 0; }
            else if(lastKey == 'i') {  mMinBlobsize--; if(mMinBlobsize < 0) mMinBlobsize = 0; }
        break;
        case '1':
            if(lastKey == 'm') { modus = 1; };
        break; 
        case '2':
            if(lastKey == 'm') { modus = 2; };
        break; 
        case '3':
            if(lastKey == 'm') { modus = 3; };
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
