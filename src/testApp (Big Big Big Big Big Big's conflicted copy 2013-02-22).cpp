#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
	looping = false;
	started = false;
	fullscreen = false;
	count = 0;
    checkerOn = false;
    
    
	
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
	
    
    // Load chroma movies
    
    chromaJohann01.setPixelFormat(OF_PIXELS_RGBA);
    chromaJohann01.loadMovie("chroma_low/chromaJohann01.mov");
    
    chromaJohann02.setPixelFormat(OF_PIXELS_RGBA);
    chromaJohann02.loadMovie("chroma_low/chromaJohann02.mov");
    
    chromaJohann03.setPixelFormat(OF_PIXELS_RGBA);
    chromaJohann03.loadMovie("chroma_low/chromaJohann03.mov");
    
    chromaJohann04.setPixelFormat(OF_PIXELS_RGBA);
    chromaJohann04.loadMovie("chroma_low/chromaJohann04.mov");
    
    chromaJohann05.setPixelFormat(OF_PIXELS_RGBA);
    chromaJohann05.loadMovie("chroma_low/chromaJohann05.mov");
    
    chromaRose01.setPixelFormat(OF_PIXELS_RGBA);
    chromaRose01.loadMovie("chroma_low/chromaRose01.mov");
    
    chromaRose02.setPixelFormat(OF_PIXELS_RGBA);
    chromaRose02.loadMovie("chroma_low/chromaRose02.mov");
    
    chromaRose03.setPixelFormat(OF_PIXELS_RGBA);
    chromaRose03.loadMovie("chroma_low/chromaRose03.mov");
    
    chromaRose04.setPixelFormat(OF_PIXELS_RGBA);
    chromaRose04.loadMovie("chroma_low/chromaRose04.mov");
    
    chromaRose05.setPixelFormat(OF_PIXELS_RGBA);
    chromaRose05.loadMovie("chroma_low/chromaRose05.mov");
    
    chromaRose06.setPixelFormat(OF_PIXELS_RGBA);
    chromaRose06.loadMovie("chroma_low/chromaRose06.mov");
    
    chromaMovieJohann = chromaJohann01;
    
    chromaMovieRose = chromaRose01;
    
    //chromaMovieRose.setPixelFormat(OF_PIXELS_RGBA);
    //chromaMovieRose.loadMovie("chroma_low/chromaRose01.mov");
    
    currentMovie = movie1;
    
    
    //play first movie
    currentMovie.play(); // play first movie
    
    

    
    
    chromaMovieJohann.play();
    chromaMovieJohann.setLoopState(OF_LOOP_NORMAL);
    
    
    
    chromaMovieRose.play();
    chromaMovieRose.setLoopState(OF_LOOP_NORMAL);
	
	
	// Turns looping off
	if (!looping) {
		currentMovie.setLoopState(OF_LOOP_NONE);
	} else {
		currentMovie.setLoopState(OF_LOOP_NORMAL);
	}
	
	ofBackground( 0, 0, 0 );
    	
    yStep = 70;
    xStep = 10;
    fbo.allocate(currentMovie.width, currentMovie.height, GL_RGBA);
    drawRuttEtra = false;

    bloomShader.load("shaders/bloom.vert", "shaders/bloom.frag");
    checkerShader.load("shaders/checkerboard.vs", "shaders/checkerboard.frag");
    kyleCheckerboard.load("shaders/kylecheckerboard.vs", "shaders/kylecheckerboard.fs");
    
    kyleCheckerboardScale = 200.0;
    
    
    xChecker = 1.0;
    yChecker = 1.0;
    
    drawFrameRate = false;
}

//--------------------------------------------------------------
void testApp::update(){
	
    currentMovie.update();
    
    if (drawRuttEtra) {
    chromaMovieJohann.update();
    chromaMovieRose.update();
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
    //    m.addFloatArg(yStep);
       // m.addFloatArg(xStep);
    
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
    
    
    
    ofScale(ofGetWidth()/currentMovie.getWidth(), -ofGetHeight()/currentMovie.getHeight());
    ofTranslate(-currentMovie.width/2, -currentMovie.height/2);
    
    if(checkerOn && !drawRuttEtra){
        
//        checkerShader.begin();
//        checkerShader.setUniformTexture("tex0",currentMovie.getTextureReference(), 1);
//        checkerShader.setUniform1f("xMultiplier", xChecker);
//        checkerShader.setUniform1f("yMultiplier", yChecker);
//        currentMovie.draw(0,0,currentMovie.getWidth(),currentMovie.getHeight());
//        checkerShader.end();
        
        kyleCheckerboard.begin();
        kyleCheckerboard.setUniformTexture("tex0",currentMovie.getTextureReference(), 1);
        kyleCheckerboard.setUniform1f("scale", kyleCheckerboardScale);
        
        currentMovie.draw(0,0,currentMovie.getWidth(),currentMovie.getHeight());
        kyleCheckerboard.end();

    }
    
    // if draw rutt etra is true, draw the rutt etra into an fbo
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
                    // mesh.addVertex(ofVec3f(x, y, z+curColor.getBrightness() * 0.3 + fftSmoothed[y]*30.f));
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
                        mesh.addVertex(ofVec3f(x, y+curColor.getBrightness() * 0.3 ));
                    } else {
                        mesh.addVertex(ofVec3f(x, y - curColor.getBrightness() * 0.3));
                    }
                    //curColor.getBrightness() * .3 + scaledVol));
                }
            }
            rowCount++;
        
        }
        fbo.begin();
        ofClear(0,0,0,0);
        mesh.draw();
        chromaMovieJohann.draw((TIME_RELATIVE)*600 -400,2*rowCount,chromaMovieJohann.width/3,chromaMovieJohann.height/3);
        chromaMovieRose.draw((TIME_RELATIVE)*600 +400 ,2*rowCount,chromaMovieRose.width/3,chromaMovieRose.height/3);
        fbo.end();
        fbo.getTextureReference(0);
        
        //if checker is on and rutt etra is on, draw the rutt etra with checker shader
        
        if (checkerOn && drawRuttEtra) {
//            checkerShader.begin();
//            checkerShader.setUniformTexture("tex0",fbo.getTextureReference(), 1);
//            checkerShader.setUniform1f("xMultiplier", xChecker);
//            checkerShader.setUniform1f("yMultiplier", yChecker);
//        fbo.draw(0,0);
//            checkerShader.end();
            
            kyleCheckerboard.begin();
            kyleCheckerboard.setUniformTexture("tex0",fbo.getTextureReference(), 1);
            kyleCheckerboard.setUniform1f("scale", kyleCheckerboardScale);
            
            fbo.draw(0,0);
            kyleCheckerboard.end();
            
            
        }else
        
            //just draw the rutt etra
            
            fbo.draw(0, 0);
}
    
   
    
    
    else{
 
        currentMovie.draw(0,0,currentMovie.getWidth(),currentMovie.getHeight());
    
    }   
   
    
    if(drawFrameRate) {
    ofSetColor(255);
    string msg = "fps: " + ofToString(ofGetFrameRate()) + "\n"
        + "yStep: " + ofToString(yStep) + "\n"
        + " xStep: " + ofToString(xStep) + "\n"
        + " xChecker: " + ofToString(xChecker) + "\n"
        + " yChecker: " + ofToString(yChecker) + "\n";
    ofDrawBitmapString(msg, 10, 20);
    }
    
    cam.end();
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    
    if(key=='w'){
        if(yStep<100)       
            yStep++;
        
        ofxOscMessage yStepMessage;
        yStepMessage.setAddress( "/movie/yStepMessage" );
        
        yStepMessage.addIntArg(yStep);
        sender1.sendMessage(yStepMessage);
        sender2.sendMessage(yStepMessage);
    }
    
    if(key=='s'){
        if(yStep>1)
            yStep--;
        ofxOscMessage yStepMessage;
        yStepMessage.setAddress( "/movie/yStepMessage" );
        
        yStepMessage.addIntArg(yStep);
        sender1.sendMessage(yStepMessage);
        sender2.sendMessage(yStepMessage);
    }
    
    
    if(key=='d'){
        if(xStep<100)       
            xStep++;
        
        ofxOscMessage xStepMessage;
        xStepMessage.setAddress( "/movie/xStepMessage" );
        
        xStepMessage.addIntArg(xStep);
        sender1.sendMessage(xStepMessage);
        sender2.sendMessage(xStepMessage);
        
    }
    
    if(key=='a'){
        if(xStep>1)
            xStep--;
        
        ofxOscMessage xStepMessage;
        xStepMessage.setAddress( "/movie/xStepMessage" );
        
        xStepMessage.addIntArg(xStep);
        sender1.sendMessage(xStepMessage);
        sender2.sendMessage(xStepMessage);
    }
    
    if(key=='r'){
        
        drawRuttEtra = !drawRuttEtra;
        
        ofxOscMessage ruttEtraMessage;
        ruttEtraMessage.setAddress( "/movie/ruttEtraMessage" );

        ruttEtraMessage.addIntArg(drawRuttEtra);
        sender1.sendMessage(ruttEtraMessage);
         sender2.sendMessage(ruttEtraMessage);
        
        
        
    }
    
   
    
    //key presses to change video
    
    
    if(key=='1'){
        movieNumber= 1;
        currentMovie.stop();
        currentMovie = movie1;
        currentMovie.play();
        
        if(drawRuttEtra){
        chromaMovieJohann.stop();
        chromaMovieJohann = chromaJohann01;
        chromaMovieJohann.play();
        
        chromaMovieRose.stop();
        chromaMovieRose = chromaRose01;
        chromaMovieRose.play();
        }
        
        
        ofxOscMessage movieNumberMessage;
        movieNumberMessage.setAddress("/movie/movieNumberMessage");
        movieNumberMessage.addIntArg(1);
        
        sender1.sendMessage(movieNumberMessage);
        sender2.sendMessage(movieNumberMessage);
        

    }
    
    if(key=='2'){
        movieNumber=2;
        currentMovie.stop();
        currentMovie = movie2;
        currentMovie.play();
        
        
        if(drawRuttEtra){

        chromaMovieJohann.stop();
        chromaMovieJohann = chromaJohann02;
        chromaMovieJohann.play();
        
        chromaMovieRose.stop();
        chromaMovieRose = chromaRose02;
        chromaMovieRose.play();
        }
        
        ofxOscMessage movieNumberMessage;
        movieNumberMessage.setAddress("/movie/movieNumberMessage");

        movieNumberMessage.addIntArg(2);
        
        sender1.sendMessage(movieNumberMessage);
        sender2.sendMessage(movieNumberMessage);
        
    }
    
    if(key=='3'){
        movieNumber=3;
        currentMovie.stop();
        currentMovie = movie3;
        currentMovie.play();
        
        if(drawRuttEtra){

        chromaMovieJohann.stop();
        chromaMovieJohann = chromaJohann03;
        chromaMovieJohann.play();
        
        chromaMovieRose.stop();
        chromaMovieRose = chromaRose03;
        chromaMovieRose.play();
        }
        
        ofxOscMessage movieNumberMessage;
        movieNumberMessage.setAddress("/movie/movieNumberMessage");

        movieNumberMessage.addIntArg(3);
       
        sender1.sendMessage(movieNumberMessage);
        sender2.sendMessage(movieNumberMessage);
        
        
    }
    
    if(key=='4'){
        movieNumber=4;
        currentMovie.stop();
        currentMovie = movie4;
        currentMovie.play();
        
        if(drawRuttEtra){

        chromaMovieJohann.stop();
        chromaMovieJohann = chromaJohann04;
        chromaMovieJohann.play();
        
        chromaMovieRose.stop();
        chromaMovieRose = chromaRose04;
        chromaMovieRose.play();
        }
        
        ofxOscMessage movieNumberMessage;
        movieNumberMessage.setAddress("/movie/movieNumberMessage");

        movieNumberMessage.addIntArg(4);
       
        sender1.sendMessage(movieNumberMessage);
        sender2.sendMessage(movieNumberMessage);
        
    }
    
    if(key=='5'){
        movieNumber=5;
        currentMovie.stop();
        currentMovie = movie5;
        currentMovie.play();
        
        if(drawRuttEtra){

        chromaMovieJohann.stop();
        chromaMovieJohann = chromaJohann05;
        chromaMovieJohann.play();
        
        chromaMovieRose.stop();
        chromaMovieRose = chromaRose05;
        chromaMovieRose.play();
        }
        
        ofxOscMessage movieNumberMessage;
        movieNumberMessage.setAddress("/movie/movieNumberMessage");

        movieNumberMessage.addIntArg(5);
        
        sender1.sendMessage(movieNumberMessage);
        sender2.sendMessage(movieNumberMessage);
        
    }
    
    if(key=='6'){
        movieNumber=6;
        currentMovie.stop();
        currentMovie = movie6;
        currentMovie.play();
        if(drawRuttEtra){

        chromaMovieJohann.stop();
        chromaMovieJohann = chromaJohann05;
        chromaMovieJohann.play();
        
        chromaMovieRose.stop();
        chromaMovieRose = chromaRose06;
        chromaMovieRose.play();
        }
        
        ofxOscMessage movieNumberMessage;
        movieNumberMessage.setAddress("/movie/movieNumberMessage");

        movieNumberMessage.addIntArg(6);
        
        sender1.sendMessage(movieNumberMessage);
        sender2.sendMessage(movieNumberMessage);
        
    }
    
    if(key=='7'){
        movieNumber=7;
        currentMovie.stop();
        currentMovie = movie7;
        currentMovie.play();
        if(drawRuttEtra){

        chromaMovieJohann.stop();
        chromaMovieJohann = chromaJohann05;
        chromaMovieJohann.play();
        
        chromaMovieRose.stop();
        chromaMovieRose = chromaRose06;
        chromaMovieRose.play();
        }
        
        ofxOscMessage movieNumberMessage;
        movieNumberMessage.setAddress("/movie/movieNumberMessage");

        movieNumberMessage.addIntArg(7);
        
        sender1.sendMessage(movieNumberMessage);
        sender2.sendMessage(movieNumberMessage);
        
    }
    
    if(key=='c'){
        
        checkerOn=!checkerOn;
        
        ofxOscMessage checkerMessage;
        checkerMessage.setAddress("/movie/checkerMessage");

        checkerMessage.addIntArg(checkerOn);
        
        
        sender1.sendMessage(checkerMessage);
        sender2.sendMessage(checkerMessage);
        
       
    }
    
    if(key=='i'){
        yChecker += 0.5;
        
    }
    
    if(key=='k'){
        yChecker -= 0.5;
        
    }
    
    if(key=='l'){
        xChecker += 0.5;
        
    }
    
    if(key=='j'){
        xChecker -= 0.5;
        
    }
    
    if(key==';'){
        drawFrameRate = !drawFrameRate;
        
    }
    
    if(key=='n'){
        kyleCheckerboardScale -=25.0;
        
        ofxOscMessage checkerScale;
        checkerScale.setAddress("/movie/checkerScale");
        
        checkerScale.addIntArg(kyleCheckerboardScale);
        
        
        sender1.sendMessage(checkerScale);
        sender2.sendMessage(checkerScale);
        
    }
    
    if(key=='m'){
        kyleCheckerboardScale +=25.0;
        
        ofxOscMessage checkerScale;
        checkerScale.setAddress("/movie/checkerScale");
        
        checkerScale.addIntArg(kyleCheckerboardScale);
        
        
        sender1.sendMessage(checkerScale);
        sender2.sendMessage(checkerScale);
        
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




