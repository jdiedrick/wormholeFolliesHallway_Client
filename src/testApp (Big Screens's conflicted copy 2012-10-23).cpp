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
    
    //rutt etra stuff
    
    yStep = 3;
    xStep = 1;
    
    drawRuttEtra = false;

}

//--------------------------------------------------------------
void testApp::update(){
	movie.idleMovie();
    
    
    
    
    
    
    if(movie.isFrameNew()){
        
        if(drawRuttEtra){
        mesh.clear();
        
        vidPixels = movie.getPixelsRef();
        
        for (int y = 0; y<movie.height; y+=yStep){
            ofNoFill();
            //  ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
            
            for (int x = 0; x < movie.width; x += xStep){
                ofColor curColor = vidPixels.getColor(x, y);
                mesh.addColor(ofColor(curColor, 255));
                mesh.addVertex(ofVec3f(x, y, curColor.getBrightness() * 0.3));
                //curColor.getBrightness() * .3 + scaledVol));
            }
        }
        
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
    
    if(drawRuttEtra){
    mesh.draw();
    }else

	// Display the movie
	movie.draw(movieX,movieY,movieWidth,movieHeight);
    
    
    
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


