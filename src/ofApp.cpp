#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255,255,255);
//	ofSetVerticalSync(true);
	frameByframe = false;
	ofSetFrameRate(30);
	// Uncomment this to show movies with alpha channels
	// fingerMovie.setPixelFormat(OF_PIXELS_RGBA);
    ofToggleFullscreen();
	FC2Version fc2Version;

    ostringstream version;
    version << "FlyCapture2 library version: " << fc2Version.major << "." << fc2Version.minor << "." << fc2Version.type << "." << fc2Version.build;
    cout << version.str() << endl;

    ostringstream timeStamp;
    timeStamp <<"Application build date: " << __DATE__ << " " << __TIME__;
    cout << timeStamp.str() << endl << endl;

     BusManager busMgr;
    unsigned int numCameras;
    error = busMgr.GetNumOfCameras(&numCameras);
    if (error != PGRERROR_OK)
    {
        PrintError( error );

    }

    PGRGuid guid;
    cout << "Number of cameras detected: " << numCameras << endl;

    for (unsigned int i=0; i < numCameras; i++)
    {

        error = busMgr.GetCameraFromIndex(i, &guid);
        if (error != PGRERROR_OK)
        {
            PrintError( error );

        }

       // RunSingleCamera( guid );
    }

    error = cam.Connect(&guid);
    if (error != PGRERROR_OK)
    {
        PrintError( error );

    }

    CameraInfo camInfo;
    error = cam.GetCameraInfo(&camInfo);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
    }

    Property camProp;
    PropertyInfo camPropInfo;

    camProp.type = FRAME_RATE;
    error = cam.GetProperty( &camProp );
    if (error != PGRERROR_OK){
        PrintError( error );
    }
    cout <<  "Frame rate is : " <<  camProp.absValue << endl;

    error = cam.StartCapture();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
    }

    error = cam.StopCapture();
    error = cam.StartCapture();

   grayImage.allocate(808,608);
   colorImage.allocate(808,608);

   switchImage.allocate(808,608,OF_IMAGE_COLOR_ALPHA);

   const char* cmd = "ffmpeg -re -f rawvideo -pix_fmt rgba  -s 808x608 -i - "" -threads 0  -y  -segment_format mkv  -vcodec huffyuv recordings/test2.mkv"; // output.mp4"; 
											
   ffmpeg = popen(cmd, "w");


}

void ofApp::releaseCamera()
{
    error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
        PrintError( error );

    }

    // Disconnect the camera
    error = cam.Disconnect();
    if (error != PGRERROR_OK)
    {
        PrintError( error );

    }

    //kinect.close();
}

//--------------------------------------------------------------
void ofApp::update(){

 error = cam.RetrieveBuffer( &rawImage );
        if (error != PGRERROR_OK)
        {
            PrintError( error );
	}

 	grayImage.resize(808,608);
      	grayImage.setFromPixels(rawImage.GetData(), 808, 608);
	grayImage.mirror(false, true);
	//colorImage = grayImage;
	switchImage.setFromPixels(grayImage.getPixels(), 808,608,OF_IMAGE_GRAYSCALE);
	switchImage.setImageType(OF_IMAGE_COLOR_ALPHA);

	fwrite(switchImage.getPixels(), sizeof(int)*808*608, 1, ffmpeg);

    //switchImage.save(ofToString(ofGetFrameNum()),OF_IMAGE_QUALITY_HIGH);

}

//--------------------------------------------------------------
void ofApp::draw(){


	//    fingerMovie.draw(20,20);
	
	switchImage.draw(0,0);
  


}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch(key){
        case 'f':
            frameByframe=!frameByframe;
            fingerMovie.setPaused(frameByframe);
        break;
        case OF_KEY_LEFT:
            fingerMovie.previousFrame();
        break;
        case OF_KEY_RIGHT:
            fingerMovie.nextFrame();
        break;
        case '0':
            fingerMovie.firstFrame();
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	if(!frameByframe){
        int width = ofGetWidth();
        float pct = (float)x / (float)width;
        float speed = (2 * pct - 1) * 5.0f;
        fingerMovie.setSpeed(speed);
	}
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if(!frameByframe){
        int width = ofGetWidth();
        float pct = (float)x / (float)width;
        fingerMovie.setPosition(pct);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(!frameByframe){
        fingerMovie.setPaused(true);
	}
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	if(!frameByframe){
        fingerMovie.setPaused(false);
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::PrintError (Error error){
    error.PrintErrorTrace();
}

void ofApp::exit(){

	pclose(ffmpeg);

}
