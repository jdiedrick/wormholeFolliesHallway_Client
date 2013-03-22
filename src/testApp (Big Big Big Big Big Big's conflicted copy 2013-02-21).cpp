#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
	looping = false;
	started = false;
	fullscreen = false;
	count = 0;
    checkerOn = false;
    drawFrameRate=false;
    
    
	
	// Load initial settings
	loadSettings("settings.xml");
	ofSetVerticalSync(true);
	
	// open an outgoing connection to HOST:PORT
	sender1.setup( host1, port1 );
	sender2.setup( host2, port2 );
	
	// Load movie files
    movie1.setPixelFormat(OF_PIXELS_RGBA);
	movie1.loadMovie("1.mov");
    
    movie2.setPixelFormat(OF_PIXELS_RGBA);
	movie2.loadMovie("2.mov");
    
    movie3.setPixelFormat(OF_PIXELS_RGBA);
	movie3.loadMovie("3.mov");
    
    movie4.setPixelFormat(OF_PIXELS_RGBA);
	movie4.loadMovie("4.mov");
    
    movie5.setPixelFormat(OF_PIXELS_RGBA);
	movie5.loadMovie("5.mov");
    
    movie6.setPixelFormat(OF_PIXELS_RGBA);
	movie6.loadMovie("6b.mov");
    
    movie7.setPixelFormat(OF_PIXELS_RGBA);
	movie7.loadMovie("7M.mov");
	
    currentMovie = movie1;
    
    //play first movie
    currentMovie.play(); // play first movie
    currentMovie.setLoopState(OF_LOOP_NORMAL);
    

    chromaMovie.setPixelFormat(OF_PIXELS_RGBA);
   // chromaMovie.loadMovie("chroma/chromad_02.mov");
    chromaMovie.loadMovie("chroma/chromad_04.mov");
    
    chromaMovie.play();
   // chromaMovie.setSpeed(2.0);
    chromaMovie.setLoopState(OF_LOOP_NORMAL);
    
    chromaMovie2.setPixelFormat(OF_PIXELS_RGBA);
//    chromaMovie2.loadMovie("chroma/chromad_rose.mov");
    chromaMovie2.loadMovie("chroma/chromad_07.mov");

    
    chromaMovie2.play();
    // chromaMovie.setSpeed(2.0);
    chromaMovie2.setLoopState(OF_LOOP_NORMAL);
	
	
	// Turns looping off
	if (!looping) {
		currentMovie.setLoopState(OF_LOOP_NONE);
	} else {
		currentMovie.setLoopState(OF_LOOP_NORMAL);
	}
	
	ofBackground( 0, 0, 0 );
    
   
	
    yStep = 1;
    xStep = 3;
   // fbo.allocate(1280, 800, GL_RGBA);
    drawRuttEtra = false;

    shader.load("shaders/bloom.vert", "shaders/bloom.frag");
    checkerShader.load("shaders/checkerboard.vs", "shaders/checkerboard.frag");
    }

//--------------------------------------------------------------
void testApp::update(){
    
    
    currentMovie.update();
    
    if(drawRuttEtra) {
    chromaMovie.update();
    
    chromaMovie2.update();
    }
    
    if(currentMovie.isFrameNew()){
        
        if(drawRuttEtra){
        
        vidPixels = currentMovie.getPixelsRef();
        
        
        }
  
    }
    
	float p = currentMovie.getPosition();
    
    
	//printf("%f\n",p);
	
	// Broadcast current position information of movie
	if ((!started || count % howOften == 0)) {
		ofxOscMessage m;
		m.setAddress( "/movie/position" );
		m.addFloatArg(p);
        m.addFloatArg(yStep);
        m.addFloatArg(xStep);
        m.addIntArg(drawRuttEtra);
        m.addIntArg(movieNumber);
        m.addIntArg(checkerOn);
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
    
    
    ofScale(ofGetWidth()/currentMovie.getWidth(), -ofGetHeight()/currentMovie.getHeight(), 2);
    

    
    // ofScale(2, 2, 2);
    
    
    //translate based on size of video
    ofTranslate(-currentMovie.width/2, -currentMovie.height/2);
    
    if(checkerOn){
        
        checkerShader.begin();
        //shader.begin();
        // Display the movie
       // if(drawRuttEtra == true){
        checkerShader.setUniformTexture("tex0",currentMovie.getTextureReference(), 1);
      //  shader.setUniformTexture("tex0",currentMovie.getTextureReference(), 1);

       // }
      //  if(drawRuttEtra == false){
        //    checkerShader.setUniformTexture("tex0",mesh.getTextureReference(), 1);
        //}
        currentMovie.draw(0,0,currentMovie.getWidth(),currentMovie.getHeight());
        checkerShader.end();
       // shader.end();
    }
//    else{
//        currentMovie.draw(0,0,currentMovie.getWidth(),currentMovie.getHeight()); 
//    }
    
    if(drawRuttEtra){
        ofMesh mesh;
        
        int rowCount = 0;
        for (int y = 0; y<currentMovie.height; y+=yStep){
            ofNoFill();
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            
            if (rowCount % 2 == 0) {
                for (int x = 0; x < currentMovie.width; x += xStep){
                    ofColor curColor = vidPixels.getColor(x, y);
                    mesh.addColor(ofColor(curColor, 255));
                    if(y<currentMovie.getWidth()/2){
                        mesh.addVertex(ofVec3f(x, y+curColor.getBrightness() * 0.3));
                    } else {
                        mesh.addVertex(ofVec3f(x, y - curColor.getBrightness() * 0.3));
                    }
                }
            } else {
                for (int x = currentMovie.width-1; x >= 0; x -= xStep){
                    ofColor curColor = vidPixels.getColor(x, y);
                    mesh.addColor(ofColor(curColor, 255));
                    if(y<currentMovie.getWidth()/2){
                        mesh.addVertex(ofVec3f(x, y+curColor.getBrightness() * 0.3));
                    } else {
                        mesh.addVertex(ofVec3f(x, y - curColor.getBrightness() * 0.3));
                    }
                }
            }
            rowCount++;
        
        }
        
    //    fbo.begin();
        ofClear(0,0,0,0);
        chromaMovie.draw((TIME_RELATIVE)*600 -400,2*rowCount,800,400);
        chromaMovie2.draw((TIME_RELATIVE)*600 +400 ,2*rowCount,800,400);
        mesh.draw();
      //  fbo.end();
        
        
    //    fbo.getTextureReference().draw(0,0);
  //      shader.begin();
        //     mesh.draw();
   //     shader.end();
          //chromaMovie.draw(400,200, 100, 300);
//        chromaMovie.draw((TIME_RELATIVE)*600 -400,2*rowCount,800,400);
//        chromaMovie2.draw((TIME_RELATIVE)*600 +400 ,2*rowCount,800,400);
  //      fbo.draw(0,0);

        /// mesh.draw();
}
    
   
    
    
    else{
  //  shader.begin();
   // checkerShader.begin();
        // Display the movie
    // checkerShader.setUniformTexture("tex0",currentMovie.getTextureReference(), 1);
        currentMovie.draw(0,0,currentMovie.getWidth(),currentMovie.getHeight());
  //  shader.end();
   // checkerShader.end();
    
    
    
   
    }   
   
    cam.end();
    


	// Display the movie
//	movie.draw(movieX,movieY,movieWidth,movieHeight);
    
    if(drawFrameRate) {
    ofSetColor(255);
    string msg = "fps: " + ofToString(ofGetFrameRate()) + "\n" + "yStep: " + ofToString(yStep) + "\n" + " xStep: " + ofToString(xStep);
    ofDrawBitmapString(msg, 10, 20);
    }

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
    
    if(key=='z'){
        chromaMovie.stop();
        chromaMovie.loadMovie("chroma/chromad_07.mov");
        chromaMovie.play();
       // chromaMovie.draw(400,rowCount,200,100);
        
        
    }
    
    if(key=='x'){
        chromaMovie.stop();
        chromaMovie.loadMovie("chroma/chromad_12.mov");
        chromaMovie.play();
    }
    
    if(key=='m'){
        chromaMovie.draw(500,200, 200,100);
    }

    
    //key presses to change video
    
    
    if(key=='1'){
        movieNumber= 1;
        currentMovie.stop();
        currentMovie = movie1;
        currentMovie.play();
        currentMovie.setLoopState(OF_LOOP_NORMAL);

    }
    
    if(key=='2'){
       
        movieNumber=2;
        currentMovie.stop();
        currentMovie = movie2;
        currentMovie.play();
        currentMovie.setLoopState(OF_LOOP_NORMAL);
    }
    
    if(key=='3'){
      
        movieNumber=3;
        currentMovie.stop();
        currentMovie = movie3;
        currentMovie.play();
        currentMovie.setLoopState(OF_LOOP_NORMAL);
        
    }
    
    if(key=='4'){
        movieNumber=4;
        currentMovie.stop();
        currentMovie = movie4;
        currentMovie.play();
        currentMovie.setLoopState(OF_LOOP_NORMAL);
    }
    
    if(key=='5'){
        movieNumber=5;
        currentMovie.stop();
        currentMovie = movie5;
        currentMovie.play();
        currentMovie.setLoopState(OF_LOOP_NORMAL);
        
    }
    
    if(key=='6'){
        movieNumber=6;
        currentMovie.stop();
        currentMovie = movie6;
        currentMovie.play();
        currentMovie.setLoopState(OF_LOOP_NORMAL);
    }
    
    if(key=='7'){
        movieNumber=7;
        currentMovie.stop();
        currentMovie = movie7;
        currentMovie.play();
        currentMovie.setLoopState(OF_LOOP_NORMAL);
        
    }
    
    if(key=='c'){
        
        checkerOn=!checkerOn;
       
    }
    
    if(key==';'){
        
        drawFrameRate=!drawFrameRate;
        
    }
  

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
  //  fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
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
