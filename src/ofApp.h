#pragma once

#include "ofMain.h"
#include "FlyCapture2.h"
#include "ofxOpenCv.h"

#define VID_LENGTH 20


using namespace FlyCapture2;

class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
		void PrintError(Error error);
		void releaseCamera();

		ofVideoPlayer 		fingerMovie;
		bool                frameByframe;
		Error error;
   		Camera cam;
    		Image rawImage;
    		Image convertedImage;
		ofVideoGrabber vidGrabber;
		ofxCvGrayscaleImage 	grayImage;
		ofxCvColorImage		colorImage;		
		FILE * ffmpeg;
  		ofImage 		switchImage;		


};

