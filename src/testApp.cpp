#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(CAMWIDTH,CAMHIGHT);


    colorImg.allocate(CAMWIDTH,CAMHIGHT);
	grayImage.allocate(CAMWIDTH,CAMHIGHT);
	grayDiff.allocate(CAMWIDTH,CAMHIGHT);
    allDiff.allocate(CAMWIDTH,CAMHIGHT);
    
    for (int i=0; i<RECORDPICTURES; i++) {
        arrSavePictures[i].allocate(CAMWIDTH,CAMHIGHT);
    }
    
    savePic = false;
    
	ofSetFrameRate(30);
    
    //Set Screen
    setupMode = true;
    speichern= false;
    
    
    //GUI
    //gui.addPage("setup");
        
    gui.addTitle("screen settings");
	gui.addSlider("screen start", scStart, -2000, 2000); 
	gui.addSlider("screen end", scStop, -2000, 2000); 
    gui.addSlider("screen height", scHeight, -200, 1500); 
    
    //VideoSetup
	gui.addTitle("video settings").setNewColumn(true);
	gui.addSlider("threshold", threshold, 0, 200);
    gui.addSlider("blur", blur, 0, 10);
    gui.addContent("gray diff", grayDiff);
    
    cout << "load settings from XLM" << endl;
    gui.loadFromXML();
	gui.show();
    
    
    //Create Fragments
    for (int i=0; i<FRAGMENTNUMBER; i++) {
        tmpFragment.create(ofRandomWidth(),ofRandomHeight());
        fragments[i] = tmpFragment;
    }
    
    startTracking = false;


    
}

//--------------------------------------------------------------
void testApp::update(){
    
   
	ofBackground(0);

    bool bNewFrame = false;

    vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();


	if (bNewFrame){


        colorImg.setFromPixels(vidGrabber.getPixels(), CAMWIDTH,CAMHIGHT);


        grayImage = colorImg;
		

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(allDiff, grayImage);
       
        grayDiff.blur(blur);
		grayDiff.threshold(threshold);
  
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}

    
    /////////////////////////////////////////////////
    // Tracking 
    // blobs
    //////////////
	n_blobs.clear();
	
	for (int i = 0; i < contourFinder.nBlobs; i++) {
		ofxCvBlob tmpBlob = contourFinder.blobs[i];        
        
		bool newblob = true;
		for (int a=0; a< m_blobs.size(); ++a) {
            if(m_blobs[a].centroid.distance(tmpBlob.centroid) < 40) {
				newblob = false;
			}	
		}
		if(newblob == true) {
			n_blobs.push_back(tmpBlob);
            // CREATE TRACKINGPOINT
            tmpTrackingPoint.create(tmpBlob.centroid);
            trackingPoints.push_back(tmpTrackingPoint);
		}
	}
	
	m_blobs.clear();
	
	for (int i = 0; i< contourFinder.nBlobs; i++) {
		ofxCvBlob tmpBlob = contourFinder.blobs[i];
		m_blobs.push_back(tmpBlob);
	}
    
    
    /////////////////////////////////////////////////
    // Tracking 
    // trackingpoints
    //////////////
    
    vector<int>blobIDs;
    int ln_tp = trackingPoints.size();
    for (int i = 0; i<ln_tp; i++) {
        int ln_mBlobs = m_blobs.size();
        float nearest;
        int nearestID;
        for (int n = 0; n < ln_mBlobs; n++) {
            //Check Abstand zwischen Blob und TP
            float distance = trackingPoints[i].checkDist(m_blobs[i].centroid);
            if(distance <= nearest) {
                distance = nearest;
                nearestID = n;
            }
        }    
        
        
        int ln_blobIDs = blobIDs.size();
        bool foundBlobID = false;
        for (int n = 0; n < ln_blobIDs; n++) {
            if(blobIDs[n] == nearestID) {
                foundBlobID = true;
                break;
            }
        }
        
        if(foundBlobID) {
            //Blob wird schon von TrackPoint verfolgt..
            //Delete trackingPoint
            trackingPoints.erase(trackingPoints.begin() + i);
            i--;
        }
        else {
            trackingPoints[i].update(m_blobs[nearestID].centroid); 
        }
    }


    
    
    
    if(savePic) { allDiff = grayImage; savePic = false; }
    
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
	
	int ln = structures.size();
	for (int i=0; i<ln; i++) {
		// pass trackingpoint coordinates
		// how to find tracking point?â
        structures[i].update((float) mouseX, (float) mouseY);
	}
       
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
    
   
    float ch = CAMHIGHT;
    float cw = CAMWIDTH;
    int breite = scStop-scStart;
    int hoehe = (int) ((ch/cw)*breite);
    ofRectangle screenRect;
    screenRect.set(scStart, scHeight-hoehe, breite, hoehe);

	if(setupMode) {
        //Draw Camera & Blobs
        ofSetHexColor(0xffffff);

        vidGrabber.draw(screenRect);
        contourFinder.draw(screenRect);
     
        
        // finally, a report:

        ofSetColor(0,255,0);
        ofDrawBitmapString("press s: kamerasetup\npress f: toggle fullscreen", 20, 650);
    
        ofSetHexColor(0xffffff);
        char reportStr[1024];
        sprintf(reportStr, "bg subtraction and blob detection\nthreshold %i \nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
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


}


void testApp::createStructure(float _x, float _y, int _n) {
	
	std::vector<Fragment*> tmp;
	
	for (int i=0; i<_n; i++) {
		Fragment f = fragments[i];
		if (!f.hasTarget) tmp.push_back(&fragments[i]);
	}
	tmpStructure.create(_x,_y,tmp);
	structures.push_back(tmpStructure);
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
        case 'w':
            speichern = true;
            break;
        case 'u':
            scStart = 0;
            gui.saveToXML();
            break;
		case 'p':
            //startTracking = true;
			createStructure(200, 600, 70);
			break;
        case ' ':
            savePic = true;
            break;

	}
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){


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
