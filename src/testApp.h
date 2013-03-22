#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"
#include "ofxTimeline.h" //add timeline include

#define HOST "localhost"
#define PORT 12345

//--------------------------------------------------------
class testApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void loadSettings(string fileString);
    
    
    
	ofVideoPlayer 		movie1;
  
    
    
    ofVideoPlayer currentMovie;
    
    int movieNumber;
    

    int mouseX;
    int mouseY;
    
    int rowCount;
	
	int count;
	int howOften;
	
	bool started;
	bool fullscreen;
	
    int port;

    
    
	
	int movieWidth;
	int movieHeight;
	int movieX;
	int movieY;
	
	bool looping;
	
	bool frameByframe;
	
   

	//osc receiver stuff
    ofxOscReceiver	receiver;


    //rutt etra stuff
    
    int yStep;
    
    int xStep;
    
    ofPixels vidPixels;
    
    ofMesh mesh;
    
    void drawMesh();
    
    bool drawRuttEtra;
        
    
    ofFbo fbo;
    ofFbo fboFloat;
    ofFbo blur;
    ofFbo comp;
    
    int fadeAmnt;
    
    
    ofShader bloomShader;
  
    void drawDebug();
    
    bool debug;

    ofxTimeline timeline; //inside of the class add a timeline

    
    
};

