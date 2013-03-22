#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	looping = false;
	started = false;
	fullscreen = false;
	count = 0;
	
	// Load initial settings
	loadSettings("settings.xml");
	ofSetVerticalSync(true);
	
	// open an outgoing connection to HOST:PORT
	sender1.setup( host1, port1 );
	sender2.setup( host2, port2 );
	
	// Load movie file
	movie.loadMovie(movieFile);
	movie.play();
	
	// Turns looping off
	if (!looping) {
		movie.setLoopState(OF_LOOP_NONE);
	} else {
		movie.setLoopState(OF_LOOP_NORMAL);
	}
	
	ofBackground( 0, 0, 0 );
    mp3.loadSound("../../../../../../../music/feeling.mp3");
    
    
    fftSmoothed = new float[8192];
	for (int i = 0; i < 8192; i++){
		fftSmoothed[i] = 0;
	}
	
	nBandsToGet = movie.getHeight();
    
    movie.setVolume(0);
    mp3.play();
    yStep = 10;
    xStep = 10;
    fbo.allocate(ofGetWindowWidth(), ofGetWindowHeight(), GL_RGBA32F_ARB);
    drawRuttEtra = true;

    

}

//--------------------------------------------------------------
void testApp::update(){
	ofSoundUpdate();
    
    float * val = ofSoundGetSpectrum(nBandsToGet);		// request 128 values for fft
	for (int i = 0;i < nBandsToGet; i++){
		
		// let the smoothed calue sink to zero:
		fftSmoothed[i] *= 0.96f;
		
		// take the max, either the smoothed or the incoming:
		if (fftSmoothed[i] < val[i]) fftSmoothed[i] = val[i];
		
	}

    
    movie.update();
    
    
    
    
    
    if(movie.isFrameNew()){
        
        if(drawRuttEtra){
        
        vidPixels = movie.getPixelsRef();
        
        
        }
  
    }
    
	float p = movie.getPosition();
    
    
	//printf("%f\n",p);
	
	// Broadcast current position information of movie
	if ((!started || count % howOften == 0)) {
		ofxOscMessage m;
		m.setAddress( "/movie/position" );
		m.addFloatArg(p);
        m.addFloatArg(yStep);
        m.addFloatArg(xStep);
        m.addIntArg(drawRuttEtra);
		sender1.sendMessage(m);
		sender2.sendMessage(m);
		started = true;
	}
	count++;
}

//--------------------------------------------------------------
void testApp::draw(){
	if (fullscreen) {
		ofHideCursor();
	}
    
    ofBackground(0);
    cam.begin();
    
    glEnable(GL_DEPTH_TEST);
    
    
    ofScale(ofGetWidth()/movie.getWidth(), -ofGetHeight()/movie.getHeight(), 2);
    
    // ofScale(2, 2, 2);
    //uncomment for video grabber
    //  ofTranslate(-vidGrabber.width/2, -vidGrabber.height/2);
    
    //translate based on size of video
    ofTranslate(-movie.width/2, -movie.height/2);
    
    
    if(drawRuttEtra){
        ofMesh mesh;
        
        int rowCount = 0;
        for (int y = 0; y<movie.height; y+=yStep){
            ofNoFill();
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            
            if (rowCount % 2 == 0) {
                for (int x = 0; x < movie.width; x += xStep){
                    ofColor curColor = vidPixels.getColor(x, y);
                    mesh.addColor(ofColor(curColor, 255));
                    // mesh.addVertex(ofVec3f(x, y, z+curColor.getBrightness() * 0.3 + fftSmoothed[y]*30.f));
                   // if(y<movie.getWidth()/2){
                    mesh.addVertex(ofVec3f(x, y+curColor.getBrightness() * 0.3) );//;
                   // } else {
                   //     mesh.addVertex(ofVec3f(x, y - curColor.getBrightness() * 0.3 - fftSmoothed[y]*50.f));
                    //}
                    //curColor.getBrightness() * .3 + scaledVol));
                }
            } else {
                for (int x = movie.width-1; x >= 0; x -= xStep){
                    ofColor curColor = vidPixels.getColor(x, y);
                    mesh.addColor(ofColor(curColor, 255));
                    // mesh.addVertex(ofVec3f(x, y, z+curColor.getBrightness() * 0.3 + fftSmoothed[y]*30.f));
                  //  if(y<movie.getWidth()/2){
                        mesh.addVertex(ofVec3f(x, y+curColor.getBrightness() * 0.3 ));
                  //  } else {
                  //      mesh.addVertex(ofVec3f(x, y - curColor.getBrightness() * 0.3 - fftSmoothed[y]*50.f));
                  //  }
                    //curColor.getBrightness() * .3 + scaledVol));
                }
            }
            rowCount++;
        }
        mesh.draw();
        /// mesh.draw();
    }else
        
        // Display the movie
        movie.draw(0,0,movie.getWidth(),movie.getHeight());
    
    
    
    cam.end();
    


	// Display the movie
//	movie.draw(movieX,movieY,movieWidth,movieHeight);
    
    //draw framerate for fun
    
    ofSetColor(255);
    string msg = "fps: " + ofToString(ofGetFrameRate()) + "\n" + "yStep: " + ofToString(yStep) + "\n" + " xStep: " + ofToString(xStep);
    ofDrawBitmapString(msg, 10, 20);
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    
    if(key=='y'){
        if(yStep<100)       
            yStep++;
        
    }
    
    if(key=='u'){
        if(yStep>1)
            yStep--;
    }
    
    
    if(key=='h'){
        if(xStep<100)       
            xStep++;
        
    }
    
    if(key=='j'){
        if(xStep>1)
            xStep--;
    }
    
    if(key=='r'){
        
        drawRuttEtra = !drawRuttEtra;
        
        
    }
       


}
//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

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

void testApp::loadSettings(string fileString){
	
	string host_address1;
	string host_address2;
	string filename;
	
	//--------------------------------------------- get configs
    ofxXmlSettings xmlReader;
	bool result = xmlReader.loadFile(fileString);
	if(!result) printf("error loading xml file\n");
	
	host_address1 = xmlReader.getValue("settings:client1:address","test",0);
	port1 = xmlReader.getValue("settings:client1:port",5204,0);
	host1 = (char *) malloc(sizeof(char)*host_address1.length());
	strcpy(host1, host_address1.c_str());
	
	host_address2 = xmlReader.getValue("settings:client2:address","test",0);
	port2 = xmlReader.getValue("settings:client2:port",5204,0);
	host2 = (char *) malloc(sizeof(char)*host_address2.length());
	strcpy(host2, host_address2.c_str());
	
	filename = xmlReader.getValue("settings:movie:","test",0);
	movieFile = (char *) malloc(sizeof(char)*filename.length());
	strcpy(movieFile, filename.c_str());
	
	howOften = xmlReader.getValue("settings:howoften",60,0);
	
	int w = xmlReader.getValue("settings:dimensions:width", 640, 0);
	int h = xmlReader.getValue("settings:dimensions:height", 480, 0);
	
	movieWidth = xmlReader.getValue("settings:dimensions:movieWidth", 640, 0);
	movieHeight = xmlReader.getValue("settings:dimensions:movieHeight", 480, 0);
	
	movieX = xmlReader.getValue("settings:dimensions:movieX", 640, 0);
	movieY = xmlReader.getValue("settings:dimensions:movieY", 480, 0);
	
	ofSetWindowShape(w, h);
	
	if(xmlReader.getValue("settings:loop", "false", 0).compare("true") == 0) {
		looping = true;
	}
	
	if(xmlReader.getValue("settings:go_fullscreen", "false", 0).compare("true") == 0) {
		fullscreen = true;
		ofSetFullscreen(true);
	}
}


