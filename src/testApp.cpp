#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);


    colorImg.allocate(320,240);
	grayImage.allocate(320,240);
	grayBg.allocate(320,240);
	grayDiff.allocate(320,240);

	ofSetFrameRate(30);
    
    //Set Screen
    setupMode = true;
    
    //scStart = 0;
    //scStop = ofGetWidth();
    //scHeight = 100;    
    
    //threshold = 80;
    
    speichern= false;
    
    
    //GUI
    gui.addTitle("screen settings");
	gui.addSlider("screen start", scStart, 0, ofGetWidth()); 
	gui.addSlider("screen end", scStop, 0, ofGetWidth()); 
    gui.addSlider("screen height", scHeight, -200, 1500); 
    
    //VideoSetup
	gui.addTitle("video settings").setNewColumn(true);
	gui.addSlider("threshold", threshold, 0, 200);
    gui.addContent("gray diff", grayDiff);
    
    cout << "load settings from XLM" << endl;
    gui.loadFromXML();
	gui.show();
    
    
    //Create Fragments
    for (int i=0; i<500; i++) {
        f.create(ofRandomWidth(),ofRandomHeight());
        fragments.push_back(f);
    }

    
}

//--------------------------------------------------------------
void testApp::update(){
    
    cout << ofNoise(ofGetElapsedTimef()) << endl;
	ofBackground(0);

    bool bNewFrame = false;

    vidGrabber.grabFrame();
	bNewFrame = vidGrabber.isFrameNew();


	if (bNewFrame){


        colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);


        grayImage = colorImg;
		

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
	}

    //Bild Speichern
    if(speichern) {
    grayBg = grayImage;	
        speichern= false;
    }
    
    //Update Fragments
    int ln = fragments.size(); // faster
    for ( int i=0; i<ln; i++ ) {
        fragments[i].update();
    }
       
}

//--------------------------------------------------------------
void testApp::draw(){

    ofSetHexColor(0xffffff);
    
    
    int breite = scStop-scStart;
    int hoehe = (int) ((3.0f/4.0f)*breite);
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
    
    ofSetColor(255, 0, 0, 50);
    for (int i = 0; i < contourFinder.nBlobs; i++){
        ofxCvBlob tmpBlob = contourFinder.blobs[i];
        int blobXMapped = ofMap(tmpBlob.centroid.x, 0, 320, scStart, scStop);
        int blobTopMapped = ofMap(tmpBlob.boundingRect.y, 0, 240, 0, screenRect.height);
        
        ofFill();
        ofRect(blobXMapped, 0, 20, scHeight-blobTopMapped);
    }

    
    //GUI
    gui.draw();
    
    
    if(!setupMode) {
        //Draw Fragments
        int ln = fragments.size(); // faster
        for ( int i=0; i<ln; i++ ) {
            fragments[i].draw();
        }
    }


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
