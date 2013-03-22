#include "testApp.h"
using namespace std;
//--------------------------------------------------------------
void testApp::setup(){
    
	looping = false;
	started = false;
	fullscreen = false;
	count = 0;
	
	// Load initial settings
	loadSettings("settings.xml");
	ofSetVerticalSync(true);
	
	// Set up OSC
	receiver.setup( port );
	std::cout << "listening for osc messages on port " << port << "\n";
	
	// Load movie files
    movie1.setPixelFormat(OF_PIXELS_RGBA);
	movie1.loadMovie("movies/movie.mov");

    currentMovie = movie1;
    
    //play first movie
    currentMovie.play();
	
	// Turns looping off
	if (!looping) {
		currentMovie.setLoopState(OF_LOOP_NONE);
	} else {
		currentMovie.setLoopState(OF_LOOP_NORMAL);
	}
	
    //set background
    ofBackground(0, 0, 0);
    
    //set initial xstep and ystep
    yStep = 10;
    xStep = 3;
    
    //allocate fbofloat
    fboFloat.allocate(currentMovie.getWidth(), currentMovie.getHeight(), GL_RGBA32F_ARB);
    
    //set draw rutt etra = false
    drawRuttEtra = false;
    
    //clear the fbo
    fboFloat.begin();
	ofClear(0,0,0,0);
    fboFloat.end();
    
    //load bloom shader
    bloomShader.load("shaders/bloom.vert", "shaders/bloom.frag");
    
    //set debug to false
    debug = false;
    
    //timeline setup
  //  timeline.setup(); //registers events
  //  timeline.setDurationInSeconds(60); //sets time
  //  timeline.setLoopType(OF_LOOP_NORMAL); //turns the timeline to loop
    
    //add a tracks, etc
  //  timeline.addCurves("yStep", ofRange(5, 100));
//    timeline.addSwitches("drawRuttEtra");
}

//--------------------------------------------------------------
void testApp::update(){
    //enable alpha blending (move to setup?)
    ofEnableAlphaBlending();

    //update current movie
    currentMovie.update();

    //if we get a new frame and if we're drawing rutt etra, get the pixels from the movie
    if(currentMovie.isFrameNew()){
        
        if(drawRuttEtra){
        
        vidPixels = currentMovie.getPixelsRef();
        
        }
  
    }
    
    //synching video stuff. remove for hallway installation. we're synching rutt etra!
//	float p = currentMovie.getPosition();
    
    
	//printf("%f\n",p);
    // Check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// Get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
		
		// Check for movie position info
		if ( m.getAddress() == "/movie/position" )
		{
            //get ystep and rutt etra
            yStep = m.getArgAsInt32(0);
            drawRuttEtra = m.getArgAsInt32(1);
//			if (!started) {
//				started = true;
//				currentMovie.play();
//				// No need to loop, we loop if the master loops
//				currentMovie.setLoopState(OF_LOOP_NONE);
//			}
		
            
            
			
		}
		
	}
    
     
	count++;

    //if rutt etra, draw our mesh into the rutt etra
    if(drawRuttEtra){
        fboFloat.begin();
        drawMesh();
        fboFloat.end();
        fboFloat.getTextureReference(0);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //draw debug screen and timeline
   
    
    //timeline code
    
  //  yStep = timeline.getValue("yStep");
    
  //  if(timeline.isSwitchOn("drawRuttEtra")){
  //      drawRuttEtra = true;
  //  }else{
   //     drawRuttEtra = false;
  //  }


//uncomment to hide cursor on fullscreen
//	if (fullscreen) {
//		ofHideCursor();
//	}
    
      
    // if draw rutt etra is true, draw the rutt etra into an fbo
    if(drawRuttEtra){

     //   ofSetColor(0,0,0);
        bloomShader.begin();
        bloomShader.setUniformTexture("texture", fboFloat.getTextureReference(), 1);
        fboFloat.draw(0,0);
        bloomShader.end();

     
}
    else if(!drawRuttEtra){
        //draw the movie
        currentMovie.draw(0,0,currentMovie.getWidth(),currentMovie.getHeight());
    } 
    
    //draw debug screen and timeline
    if(debug) {
        drawDebug();
    }
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    
       
    if(key=='r'){
        
        drawRuttEtra = !drawRuttEtra;
        
        
    }

    if(key=='d'){
        debug = !debug;
        
    }  

}



void testApp::loadSettings(string fileString){
	
	string host_address1;
	string host_address2;
    string host_address3;
	string host_address4;
    string host_address5;
	string host_address6;
    
	string filename;
	
	//--------------------------------------------- get configs
    ofxXmlSettings xmlReader;
	bool result = xmlReader.loadFile(fileString);
	if(!result) printf("error loading xml file\n");
	
    port = xmlReader.getValue("settings:port",9999,0);

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

void testApp::drawMesh(){
    //most of this is based off the fbo trails example :)
    
    //we clear the fbo if c is pressed.
	//this completely clears the buffer so you won't see any trails
	if( ofGetKeyPressed('c') ){
		ofClear(0,0,0, 0);
	}
	
	//some different alpha values for fading the fbo
	//the lower the number, the longer the trails will take to fade away.
	fadeAmnt = 40;
	if(ofGetKeyPressed('1')){
		fadeAmnt = 1;
	}else if(ofGetKeyPressed('2')){
		fadeAmnt = 5;
	}else if(ofGetKeyPressed('3')){
		fadeAmnt = 15;
	}
    
    ofPushStyle();
    ofFill();
	ofSetColor(0,0,0, fadeAmnt);
	ofRect(0,0,currentMovie.getWidth(),currentMovie.getHeight());
    ofPopStyle();
    
    ofMesh mesh;
    
    int rowCount = 0;
    for (int y = 0; y<currentMovie.height; y+=yStep){
        //   ofNoFill();
        mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
        
        if (rowCount % 2 == 0) {
            for (int x = 0; x < currentMovie.width; x += xStep){
                ofColor curColor = vidPixels.getColor(x, y);
                mesh.addColor(ofColor(curColor, 255));
                // mesh.addVertex(ofVec3f(x, y, z+curColor.getBrightness() * 0.3 + fftSmoothed[y]*30.f));
                if(y<currentMovie.getWidth()/2){
                    mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3));
                } else {
                    mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3));
                }
            }
        } else {
            for (int x = currentMovie.width-1; x >= 0; x -= xStep){
                ofColor curColor = vidPixels.getColor(x, y);
                mesh.addColor(ofColor(curColor, 255));
                if(y<currentMovie.getWidth()/2){
                    mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3 ));
                } else {
                    mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3));
                }
            }
        }
        rowCount++;
        
    }
    mesh.draw();
}

void testApp::drawDebug(){
    
    ofSetColor(255);
    string msg = "fps: " + ofToString(ofGetFrameRate()) + "\n"
    + "yStep: " + ofToString(yStep) + "\n"
    + " xStep: " + ofToString(xStep) + "\n";
    ofDrawBitmapString(msg, 10, 20);
    timeline.draw();

    
}

