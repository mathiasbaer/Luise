#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    
    //General Settings
    ofSetFrameRate(30);
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    //Grafik Settings
    ofBackground(0);
    
    //Videoinput & Tracking
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(CAMWIDTH,CAMHEIGHT);
    
    mColorImg.allocate(CAMWIDTH,CAMHEIGHT);
    mGrayImage.allocate(CAMWIDTH,CAMHEIGHT);
    mGrayDiff.allocate(CAMWIDTH,CAMHEIGHT);
    mSaveBackground.allocate(CAMWIDTH,CAMHEIGHT);
    

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
    gui.addContent("gray diff", mGrayDiff);
    
    gui.loadFromXML();
	gui.show();
    

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
    
    
    
    
    //Build Structurs
    int ln_tp = trackingPoints.size();
    for (int i = 0; i<ln_tp; i++) {        
        if(!trackingPoints[i].hasStructure) {
            cout << "struktur erstellen " << endl;
            //struktur erstellen
            
            ofVec2f mappedPosition;
            mappedPosition.x = ofMap(trackingPoints[i].position.x, 0, CAMWIDTH, 0, ofGetWidth());
            mappedPosition.y = ofMap(trackingPoints[i].position.y, 0, CAMHEIGHT, 0, ofGetHeight());
            
            createStructure(mappedPosition, ofRandom(50-80));
        }
    }
	
	
    int ln = structures.size();
	for (int i=0; i<ln; i++) {
		// pass trackingpoint coordinates
		// how to find tracking point?â
        //structures[i].update((float) mouseX, (float) mouseY);
        structures[i].update( &trackingPoints );
	}
    
    
   
    
    /*
    //Save Picture
    int picNr = ofGetFrameNum()%RECORDPICTURES;
    arrSavePictures[picNr] = grayImage;
    
    ofxCvGrayscaleImage 	tmpGrayImg;
    tmpGrayImg.allocate(CAMWIDTH, CAMHIGHT);
    tmpGrayImg = grayDiff;
    
    for(int i=0;i<RECORDPICTURES;i++) {
        unsigned char * mergePixels = maxImage(tmpGrayImg,arrSavePictures[i]);
        tmpGrayImg.setFromPixels(mergePixels, CAMWIDTH, CAMHIGHT);
        
    }
    
    allDiff = tmpGrayImg;
     
     */
    
    
    //Update Fragments
    for ( int i=0; i<FRAGMENTNUMBER; i++ ) {
        fragments[i].update();
    }
	

    
    /*
       
    //Tracking
    if(startTracking) {
        
        //blobs
        //if(contourFinder.nBlobs.size() > 0) {
        
        //}
        for (int i = 0; i < contourFinder.nBlobs; i++){
            ofxCvBlob tmpBlob = contourFinder.blobs[i];
            float blobXMapped = ofMap(tmpBlob.centroid.x, 0, CAMWIDTH, 0, ofGetWidth());
            float blobYMapped = ofMap(tmpBlob.centroid.y, 0, CAMHIGHT, 0, ofGetHeight());
    
        }
    
    }
     */
    
}

//--------------------------------------------------------------
//Die Funktion ist noch nicht optimal, aber funktioniert schon... besser wŠre, wenn man direkt das Bilderarray Ÿbergibt und er alle Bilder addiert...
unsigned char* testApp::maxImage( ofxCvGrayscaleImage& mom, ofxCvGrayscaleImage& dad ) {

    int totalPixels = mom.width * mom.height;  
    unsigned char * momPixels = mom.getPixels();  
    unsigned char * dadPixels = dad.getPixels();  
    
    for(int i=0; i<totalPixels; i++) {  
        momPixels[i] = std::max(momPixels[i],dadPixels[i]);
    }
    
    return momPixels;

    
}

//--------------------------------------------------------------
void testApp::draw(){

    ofSetHexColor(0xffffff);

   //allDiff.draw(0, 100);
    
   
    float ch = CAMHEIGHT;
    float cw = CAMWIDTH;
    int breite = scStop-scStart;
    int hoehe = (int) ((ch/cw)*breite);
    ofRectangle screenRect;
    screenRect.set(scStart, scHeight-hoehe, breite, hoehe);

	if(setupMode) {
        //Draw Camera & Blobs
        ofSetHexColor(0xffffff);

        vidGrabber.draw(screenRect);
        mContourFinder.draw(screenRect);
     
        
        // finally, a report:

        ofSetColor(0,255,0);
        ofDrawBitmapString("press s: kamerasetup\npress f: toggle fullscreen", 20, 650);
    
        ofSetHexColor(0xffffff);
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
    gui.draw();
    
    
    if(!setupMode) {
        //Draw Fragments
        for ( int i=0; i<FRAGMENTNUMBER; i++ ) {
            fragments[i].draw();
        }
    }
    
   
    
    ofSetColor(255);
    mGrayDiff.draw(0, 0);
    mContourFinder.draw();
    
    vidGrabber.draw(400,0);

     
    //Trackingpoints zeichnen
    int ln_tp = trackingPoints.size();
    for (int i = 0; i<ln_tp; i++) {
        trackingPoints[i].draw();
    }
 

}


void testApp::createStructure(ofVec2f _pos, int _n) {
	
	std::vector<Fragment*> tmp;
	
	for (int i=0; i<_n; i++) {
		Fragment f = fragments[i];
		if (!f.hasTarget) tmp.push_back(&fragments[i]);
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
            gui.toggleDraw();
            setupMode =! setupMode;
            break;
        case 'u':
            scStart = 0;
            gui.saveToXML();
            break;
		case 'p':
            //startTracking = true;
			createStructure(ofVec2f(200, 600), 70);
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
