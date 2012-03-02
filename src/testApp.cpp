#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    
    //General Settings
    ofSetFrameRate(30);

    
    //Grafik Settings
    ofEnableAlphaBlending();
    //ofEnableSmoothing();
    ofBackground(0, 0, 0);
    ofSetBackgroundAuto(false);
    
    
    //Videoinput & Tracking
    vidGrabber.setVerbose(true);
	//vidGrabber.setDeviceID(3);
    vidGrabber.initGrabber(CAMWIDTH,CAMHEIGHT);
    
    mColorImg.allocate(CAMWIDTH,CAMHEIGHT);
    mGrayImage.allocate(CAMWIDTH,CAMHEIGHT);
    mGrayDiff.allocate(CAMWIDTH,CAMHEIGHT);
    mSaveBackground.allocate(CAMWIDTH,CAMHEIGHT);
    
	imageList.create();
	
	graphic.create();
	
	
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
    
    vidGrabber.grabFrame();
    
    //Neuer Frame?
    bool bNewFrame = false;
    bNewFrame = vidGrabber.isFrameNew();

    if (bNewFrame){
        
        mColorImg.setFromPixels(vidGrabber.getPixels(), CAMWIDTH,CAMHEIGHT);
        mGrayImage = mColorImg;
        
        // take the abs value of the difference between background and incoming and then threshold:
		mGrayDiff.absDiff(mSaveBackground, mGrayImage);
        
        mGrayDiff.blur(mBlur);
		mGrayDiff.threshold(mThreshold);
        
        // find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		mContourFinder.findContours(mGrayDiff, 20, (340*240)/3, 10, false);	// find holes
        
        
        
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
                    trackingPoints.push_back(TrackingPoint(tmpBlob.centroid));
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
        fragments[i].update();
    }
	
	
	/////////////////////////////////////////////////
    // GraphicsContainer ////////////////////////////
    /////////////////////////////////////////////////	

    graphic.update();
}

//--------------------------------------------------------------
void testApp::draw(){

    ofSetColor(0,0,0,50);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
    
    float ch = CAMHEIGHT;
    float cw = CAMWIDTH;
    int breite = scStop-scStart;
    int hoehe = (int) ((ch/cw)*breite);
    ofRectangle screenRect;
    screenRect.set(scStart, scHeight-hoehe, breite, hoehe);
   

	

    if(setupMode) {

        //Draw Camera & Blobs
        ofSetColor(255,255,255);
        
        vidGrabber.draw(screenRect);
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
        for ( int i=0; i<FRAGMENTNUMBER; i++ ) {
            fragments[i].draw();
		}
   
    
	
		ofSetColor(255);
		mGrayDiff.draw(0, 0);
		mContourFinder.draw();
    
		vidGrabber.draw(400,0);
		
    }
    

     
    //Trackingpoints zeichnen
    int ln_tp = trackingPoints.size();
    for (int i = 0; i<ln_tp; i++) {
        trackingPoints[i].draw();
        
    }
	
	

	// GrafphicsContainer
	graphic.draw();
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


//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
        case 's':
            vidGrabber.videoSettings();
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

	graphic.setImage(imageList.getRandom());
	graphic.setPosition(mouseX, mouseY);
	
	drawBuffer = true;
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
