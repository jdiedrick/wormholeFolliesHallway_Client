#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
//#include "ofxAlphaVideoPlayer.h"

#define HOST "localhost"
#define PORT 12345

//--------------------------------------------------------
class testApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void loadSettings(string fileString);
    
    
    
	ofVideoPlayer 		movie1;
    ofVideoPlayer       movie2;
    ofVideoPlayer       movie3;
    ofVideoPlayer       movie4;
    ofVideoPlayer       movie5;
    ofVideoPlayer       movie6;
    ofVideoPlayer       movie7;
    
    ofVideoPlayer currentMovie;
    
    int movieNumber;
    
    
    ofVideoPlayer 		chromaMovie;
    ofVideoPlayer 		chromaMovie2;

    int mouseX;
    int mouseY;
    
    int rowCount;
	
	int count;
	int howOften;
	
	bool started;
	bool fullscreen;
	
	char * movieFile;
	char * host1;
	char * host2;
	int port1;
	int port2;
	
	int movieWidth;
	int movieHeight;
	int movieX;
	int movieY;
	
	bool looping;
	
	bool frameByframe;
	
    int nBandsToGet;

	//osc stuff
	ofxOscSender sender1;
	ofxOscSender sender2;
    //rutt etra stuff
    
    int yStep;
    
    int xStep;
    
    ofPixels vidPixels;
    
    ofMesh mesh;
    
    bool drawRuttEtra;
    
    bool checkerOn;
    
    ofSoundPlayer mp3;
    
    ofFbo fbo;
    
    float 				* fftSmoothed;
    
    ofEasyCam cam;
    
    ofShader shader;
    ofShader checkerShader;
    
    bool drawFrameRate;
    
    
};

